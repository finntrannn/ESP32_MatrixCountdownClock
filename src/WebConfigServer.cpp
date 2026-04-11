/**
 * @file WebConfigServer.cpp
 * @brief Implementation of the web-based configuration portal.
 *
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#include "WebConfigServer.h"

#include <Update.h>

#include "WebStrings.h"

static volatile bool shouldReboot_ = false;

String getSlider(const String &name, int min, int max, int val) {
	return "<div class='sg'>"
		   "<input type='range' id='r_" +
		   name + "' name='" + name + "' min='" + String(min) + "' max='" +
		   String(max) + "' value='" + String(val) + "' oninput='syncS(\"n_" +
		   name +
		   "\", this.value)'>"
		   "<input type='number' id='n_" +
		   name + "' min='" + String(min) + "' max='" + String(max) +
		   "' value='" + String(val) + "' oninput='syncS(\"r_" + name +
		   "\", this.value)'></div>";
}

void WebConfigServer::begin(AppState &state, DisplayManager &display,
							SplashScreen &splash, FireworksEffect &fireworks) {
	state_	   = &state;
	display_   = &display;
	splash_	   = &splash;
	fireworks_ = &fireworks;

	server_.on("/", HTTP_GET,
			   [this](AsyncWebServerRequest *request) { handleRoot(request); });
	server_.on("/save", HTTP_POST,
			   [this](AsyncWebServerRequest *request) { handleSave(request); });
	server_.on("/restart", HTTP_POST, [this](AsyncWebServerRequest *request) {
		handleRestart(request);
	});
	server_.on(
		"/showsplash", HTTP_POST,
		[this](AsyncWebServerRequest *request) { handleShowSplash(request); });
	server_.on("/test-fireworks", HTTP_POST,
			   [this](AsyncWebServerRequest *request) {
				   handleTestFireworks(request);
			   });

	// Spinner AJAX endpoints
	server_.on("/spin", HTTP_POST, [this](AsyncWebServerRequest *request) {
		state_->requestSpin();
		request->send(200, "text/plain", "OK");
	});
	server_.on("/spin-reset", HTTP_POST, [this](AsyncWebServerRequest *request) {
		state_->requestSpinReset();
		request->send(200, "text/plain", "OK");
	});

	server_.on(
		"/update", HTTP_POST,
		[](AsyncWebServerRequest *request) {
			shouldReboot_					 = !Update.hasError();
			AsyncWebServerResponse *response = request->beginResponse(
				200, "text/plain", shouldReboot_ ? "OK" : "FAIL");
			response->addHeader("Connection", "close");
			request->send(response);
		},
		[](AsyncWebServerRequest *request, String filename, size_t index,
		   uint8_t *data, size_t len, bool final) {
			if (!index) {
				Serial.printf("Update: %s\n", filename.c_str());
				if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
					Update.printError(Serial);
				}
			}
			if (!Update.hasError()) {
				if (Update.write(data, len) != len) {
					Update.printError(Serial);
				}
			}
			if (final) {
				if (Update.end(true)) {
					Serial.printf("Update Success: %uB\n", index + len);
				} else {
					Update.printError(Serial);
				}
			}
		});

	server_.onNotFound([](AsyncWebServerRequest *request) {
		request->redirect("http://192.168.4.1/");
	});

	server_.begin();
}

void WebConfigServer::loop() {
	if (shouldReboot_) {
		shouldReboot_ = false;
		delay(500);
		ESP.restart();
	}
}

void WebConfigServer::handleRoot(AsyncWebServerRequest *request) {
	using namespace WebUI;

	time_t targetEpoch = state_->getTargetEpoch();
	struct tm tm_targ_info;
	struct tm *tm_targ = localtime_r(&targetEpoch, &tm_targ_info);
	char dtStr[64];
	snprintf(dtStr, sizeof(dtStr), "%04d-%02d-%02dT%02d:%02d",
			 tm_targ->tm_year + 1900, tm_targ->tm_mon + 1, tm_targ->tm_mday,
			 tm_targ->tm_hour, tm_targ->tm_min);

	String html;
	html.reserve(20000);
	html +=
		"<!DOCTYPE html><html><head><meta charset='utf-8'><meta "
		"name='viewport' content='width=device-width, initial-scale=1, "
		"maximum-scale=1, user-scalable=no'><title>";
	html += kPageTitle;
	html += "</title>";
	html += R"rawhtml(
<style>
*{box-sizing:border-box;}
body{font-family:'Segoe UI',Roboto,Helvetica,sans-serif;margin:0;padding:0;background:#f0f2f5;color:#1c1e21;display:flex;height:100vh;overflow:hidden;}

.sidebar{width:260px;background:#fff;display:flex;flex-direction:column;overflow-y:auto;box-shadow:2px 0 8px rgba(0,0,0,.05);z-index:20;border-right:1px solid #ddd;}
.sidebar-header{padding:20px;color:#0056b3;display:flex;align-items:center;border-bottom:1px solid #eee;}
.sidebar-header h2{margin:0;font-size:18px;font-weight:700;}

.dropdown-btn{background:none;border:none;color:#1c1e21;padding:14px 18px;text-align:left;font-size:14px;cursor:pointer;font-weight:700;display:flex;justify-content:space-between;align-items:center;border-bottom:1px solid #f0f2f5;outline:none;transition:.15s;}
.dropdown-btn:hover{background:#f5f6f7;}
.dropdown-icon{transition:transform .2s;font-size:11px;color:#999;}
.dropdown-active .dropdown-icon{transform:rotate(180deg);color:#0056b3;}
.dropdown-container{display:none;flex-direction:column;background:#fafafa;}

.tab-btn{background:none;border:none;color:#4b4f56;padding:12px 18px 12px 36px;text-align:left;font-size:13px;cursor:pointer;border-left:3px solid transparent;font-weight:600;transition:.15s;}
.tab-btn:hover{background:#f0f2f5;color:#0056b3;}
.tab-btn.active{background:#e7f3ff;color:#0056b3;border-left:3px solid #0056b3;}

.flat-btn{background:none;border:none;color:#4b4f56;padding:14px 18px;text-align:left;font-size:14px;cursor:pointer;font-weight:700;border-bottom:1px solid #f0f2f5;border-left:3px solid transparent;transition:.15s;}
.flat-btn:hover{background:#f5f6f7;color:#0056b3;}
.flat-btn.active{background:#e7f3ff;color:#0056b3;border-left:3px solid #0056b3;}

.main-content{flex:1;padding:24px;overflow-y:auto;display:flex;flex-direction:column;}
.top-actions{display:flex;gap:10px;justify-content:flex-end;margin-bottom:20px;padding:12px;background:#fff;border-radius:10px;border:1px solid #ddd;box-shadow:0 1px 3px rgba(0,0,0,.05);}
.btn-small{padding:9px 16px;font-size:13px;font-weight:bold;border-radius:6px;border:none;cursor:pointer;transition:.2s;color:#fff;}
.btn-small:hover{opacity:.9;transform:translateY(-1px);}
.tab-content{display:none;animation:fadeIn .25s ease;}
@keyframes fadeIn{from{opacity:0;transform:translateY(4px);}to{opacity:1;transform:translateY(0);}}
.grid{display:grid;gap:18px;grid-template-columns:1fr;}
@media(min-width:1000px){.grid{grid-template-columns:1fr 1fr;}}

.section{background:#fff;padding:22px;border-radius:12px;height:fit-content;box-shadow:0 2px 8px rgba(0,0,0,.04);border:1px solid #eaeaea;}
.section h3{margin:0 0 18px;font-size:16px;color:#1c1e21;border-bottom:2px solid #f0f2f5;padding-bottom:10px;font-weight:700;}
input:not([type='file']):not([type='radio']):not([type='checkbox']):not([type='range']),select{box-sizing:border-box;width:100%;padding:10px 8px 10px 10px;margin:4px 0 16px;border-radius:8px;border:1px solid #ccd0d5;background:#fff;color:#1c1e21;font-size:15px;outline:none;transition:border .2s;}
input[type='color']{height:46px;padding:3px;cursor:pointer;}
input[type='color']::-webkit-color-swatch-wrapper{padding:0;}
input[type='color']::-webkit-color-swatch{border:none;border-radius:5px;}
input:focus,select:focus{border-color:#0056b3;}

.sg{display:flex;align-items:center;gap:8px;margin:4px 0 16px;}
.sg input[type='range']{flex:1 1 auto;min-width:60px;height:6px;margin:0;cursor:pointer;accent-color:#0056b3;}
.sg input[type='number']{flex:0 0 64px;width:64px;margin:0;text-align:center;padding:6px 4px;font-size:13px;border-radius:6px;border:1px solid #ccd0d5;}

label{display:block;font-size:13px;color:#606770;font-weight:600;margin-bottom:4px;}
.radio-group{display:flex;flex-direction:column;gap:8px;margin-bottom:16px;}
.radio-group label{display:flex;align-items:center;padding:14px;border-radius:8px;cursor:pointer;background:#fdfdfd;border:1px solid #ddd;margin:0;font-weight:600;font-size:14px;transition:.2s;color:#4b4f56;}
.radio-group label.selected{background:#e7f3ff;border-color:#0056b3;color:#0056b3;}
.radio-group input{margin:0 10px 0 0;width:16px;height:16px;accent-color:#0056b3;}
.chk{display:flex;align-items:center;cursor:pointer;margin:10px 0;background:#fdfdfd;padding:14px;border-radius:8px;border:1px solid #ddd;gap:10px;font-weight:600;font-size:13px;color:#1c1e21;transition:.2s;}
.chk:hover{background:#f5f6f7;}
.chk input{width:18px;height:18px;margin:0;cursor:pointer;accent-color:#0056b3;}
.note{font-size:12px;color:#777;margin-top:-6px;margin-bottom:16px;display:block;line-height:1.4;}
progress{width:100%;height:18px;margin-top:8px;display:none;}

/* ─── Hamburger (mobile) ─── */
.hamburger{display:none;background:#0056b3;color:#fff;border:none;border-radius:8px;width:40px;height:40px;flex-shrink:0;font-size:22px;cursor:pointer;box-shadow:0 2px 6px rgba(0,0,0,.15);line-height:1;transition:opacity .2s;margin-bottom:10px;}
.overlay{display:none;position:fixed;inset:0;background:rgba(0,0,0,.35);z-index:15;}

@media(max-width:768px){
  .hamburger{display:block;}
  .sidebar{position:fixed;left:-300px;top:0;bottom:0;width:260px;transition:left .25s ease;z-index:30;}
  .sidebar.open{left:0;}
  .sidebar.open~.overlay+.main-content .hamburger,.hamburger.hidden{opacity:0;pointer-events:none;}
  .overlay.show{display:block;}
  .main-content{padding:14px;}
  .top-actions{flex-wrap:wrap;gap:6px;}
  .top-actions>form,.top-actions>button{flex:1;min-width:0;}
  .btn-small{width:100%;padding:10px 6px;font-size:12px;}
}
</style>
<script>
function toggleMenu(){
  var s=document.getElementById('sidebar');
  var h=document.getElementById('hbtn');
  s.classList.toggle('open');
  document.getElementById('overlay').classList.toggle('show');
  if(s.classList.contains('open')){h.classList.add('hidden');}else{h.classList.remove('hidden');}
}
function closeMenu(){
  document.getElementById('sidebar').classList.remove('open');
  document.getElementById('overlay').classList.remove('show');
  document.getElementById('hbtn').classList.remove('hidden');
}
function toggleDropdown(btn){
  btn.classList.toggle("dropdown-active");
  var c=btn.nextElementSibling;
  c.style.display=c.style.display==="flex"?"none":"flex";
}
function opentab(t,parentBtnId){
  var c=document.getElementsByClassName('tab-content');
  for(var i=0;i<c.length;i++) c[i].style.display='none';
  var b=document.getElementsByClassName('tab-btn');
  for(var j=0;j<b.length;j++) b[j].classList.remove('active');
  var f=document.getElementsByClassName('flat-btn');
  for(var k=0;k<f.length;k++) f[k].classList.remove('active');
  document.getElementById('tab_'+t).style.display='block';
  var btn=document.getElementById('btn_'+t);
  if(btn) btn.classList.add('active');
  var a=document.getElementById('activeTabInput');
  if(a)a.value=t;
  if(parentBtnId){
    var p=document.getElementById(parentBtnId);
    if(p&&p.nextElementSibling&&p.nextElementSibling.style.display!=="flex"){
      p.classList.add("dropdown-active");
      p.nextElementSibling.style.display="flex";
    }
  }
  if(window.innerWidth<=768) closeMenu();
}
function selRadio(name,val){
  var l=document.getElementsByTagName('label');
  for(var i=0;i<l.length;i++) if(l[i].getAttribute('data-rn')===name)l[i].className='';
  document.getElementById('lbl_'+name+'_'+val).className='selected';
}
function syncS(id,v){document.getElementById(id).value=v;}
function doSpin(btn) {
  var durInput = document.querySelector('input[name="spinDur"]');
  var dur = (durInput ? parseInt(durInput.value) : 5) * 1000;
  btn.disabled = true;
  var oldText = btn.innerText;
  btn.innerText = "Đang quay...";
  btn.style.opacity = "0.5";
  fetch("/spin", {method:"POST"});
  setTimeout(function() {
    btn.disabled = false;
    btn.innerText = oldText;
    btn.style.opacity = "1";
  }, dur + 2500);
}
function doSpinReset(btn) {
  fetch("/spin-reset", {method:"POST"});
}
</script>
)rawhtml";

	String currentTab =
		request->hasParam("tab") ? request->getParam("tab")->value() : "sys";
	String parentId = "";
	if (currentTab == "cd" || currentTab == "sensor" ||
		currentTab == "datetime" || currentTab == "text" ||
		currentTab == "spinner" || currentTab == "scr_gen")
		parentId = "dd_disp";

	html += "<script>window.onload=function(){opentab('" + currentTab + "'" +
			(parentId.length() > 0 ? ",'" + parentId + "'" : "") +
			");};</script>";
	html += "</head><body>";

	html += "<div class='overlay' id='overlay' onclick='closeMenu()'></div>";

	html += "<div class='sidebar' id='sidebar'>";
	html += "<div class='sidebar-header'><h2>";
	html += kSidebarTitle;
	html += "</h2></div>";

	// Dropdown: Màn Hình Hiển Thị
	html +=
		"<button type='button' class='dropdown-btn' "
		"onclick='toggleDropdown(this)' id='dd_disp'>";
	html += kNavDisplay;
	html += " <span class='dropdown-icon'>&#9660;</span></button>";
	html += "<div class='dropdown-container'>";
	html +=
		"<button type='button' id='btn_scr_gen' class='tab-btn' "
		"onclick='opentab(\"scr_gen\",\"dd_disp\")'>";
	html += kNavGeneral;
	html += "</button>";
	html +=
		"<button type='button' id='btn_cd' class='tab-btn' "
		"onclick='opentab(\"cd\",\"dd_disp\")'>";
	html += kNavCountdown;
	html += "</button>";
	html +=
		"<button type='button' id='btn_sensor' class='tab-btn' "
		"onclick='opentab(\"sensor\",\"dd_disp\")'>";
	html += kNavSensor;
	html += "</button>";
	html +=
		"<button type='button' id='btn_datetime' class='tab-btn' "
		"onclick='opentab(\"datetime\",\"dd_disp\")'>";
	html += kNavDateTime;
	html += "</button>";
	html +=
		"<button type='button' id='btn_text' class='tab-btn' "
		"onclick='opentab(\"text\",\"dd_disp\")'>";
	html += kNavText;
	html += "</button>";
	html +=
		"<button type='button' id='btn_spinner' class='tab-btn' "
		"onclick='opentab(\"spinner\",\"dd_disp\")'>";
	html += kNavSpinner;
	html += "</button>";
	html += "</div>";

	// Flat: WiFi
	html +=
		"<button type='button' id='btn_sys' class='flat-btn' "
		"onclick='opentab(\"sys\")'>";
	html += kNavWifi;
	html += "</button>";

	// Flat: Tự Động & Lịch
	html +=
		"<button type='button' id='btn_auto' class='flat-btn' "
		"onclick='opentab(\"auto\")'>";
	html += kNavAutomation;
	html += "</button>";

	// Flat: OTA
	html +=
		"<button type='button' id='btn_ota' class='flat-btn' "
		"onclick='opentab(\"ota\")'>";
	html += kNavOta;
	html += "</button>";

	html += "</div>";  // end sidebar

	html += "<div class='main-content'>";
	html +=
		"<button class='hamburger' id='hbtn' "
		"onclick='toggleMenu()'>&#9776;</button>";
	html += "<div class='top-actions'>";
	html +=
		"<button type='submit' form='mForm' class='btn-small' "
		"style='background:#0056b3;'>";
	html += kBtnSave;
	html += "</button>";
	html +=
		"<form action='/showsplash' method='POST' style='margin:0;'><button "
		"type='submit' class='btn-small' style='background:#f39c12;'>";
	html += kBtnTestSplash;
	html += "</button></form>";
	html += "<form action='/restart' method='POST' onsubmit='return confirm(\"";
	html += kConfirmRestart;
	html +=
		"\");' style='margin:0;'><button type='submit' class='btn-small' "
		"style='background:#dc3545;'>";
	html += kBtnRestart;
	html += "</button></form></div>";

	html += "<form id='mForm' action='/save' method='POST'>";
	html +=
		"<input type='hidden' id='activeTabInput' name='activeTab' "
		"value='sys'>";

	// ─── TAB: WiFi ───────────────────────────────────────────────
	html += "<div id='tab_sys' class='tab-content'><div class='grid'>";
	html += "<div class='section'><h3>" + String(kWifiStaTitle) + "</h3>";
	html += "<label>" + String(kWifiStaSsid) +
			"</label><input type='text' name='staSSID' value='" +
			state_->getStaSSID() + "' required>";
	html += "<label>" + String(kWifiStaPass) +
			"</label><input type='text' name='staPass' value='" +
			state_->getStaPass() + "'>";
	html += "</div>";
	html += "<div class='section'><h3>" + String(kWifiApTitle) + "</h3>";
	html += "<label>" + String(kWifiApSsid) +
			"</label><input type='text' name='apSSID' value='" +
			state_->getApSSID() + "' required>";
	html += "<label>" + String(kWifiApPass) +
			"</label><input type='text' name='apPass' value='" +
			state_->getApPass() + "' required>";
	html += "</div></div></div>";

	// ─── TAB: General ────────────────────────────────────────────
	html += "<div id='tab_scr_gen' class='tab-content'><div class='grid'>";
	html += "<div class='section'><h3>" + String(kGenTitle) + "</h3>";
	html +=
		"<label class='chk'><input type='checkbox' name='ledState' value='1' " +
		String(state_->isLedEnabled() ? "checked" : "") + "> " + kGenLedOn +
		"</label>";
	html +=
		"<label class='chk'><input type='checkbox' name='splashState' "
		"value='1' " +
		String(state_->isSplashEnabled() ? "checked" : "") + "> " +
		kGenSplashOn + "</label>";
	html += "<label>" + String(kGenSplashText) +
			"</label><input type='text' name='splashText' value='" +
			state_->getSplashText() + "' maxlength='20'>";
	html +=
		"<label class='chk'><input type='checkbox' name='wifiIcon' value='1' " +
		String(state_->isWifiIconEnabled() ? "checked" : "") + "> " +
		kGenWifiIcon + "</label>";
	html += "<label>" + String(kGenBrightness) + "</label>" +
			getSlider("brightness", 1, 255, state_->getBrightness());
	html += "</div>";
	html += "<div class='section'><h3>" + String(kGenScreenTitle) + "</h3>";
	html += "<span class='note'>" + String(kGenScreenNote) + "</span>";
	html += "<div class='radio-group'>";
	for (int i = 0; i < 5; i++) {
		const char *labels[] = {kScrCountdown, kScrSensor, kScrDateTime,
								kScrText, kScrSpinner};
		html += "<label data-rn='mode' id='lbl_mode_" + String(i) +
				"' class='" +
				String(state_->getScreenMode() == i ? "selected" : "") +
				"' onclick='selRadio(\"mode\"," + String(i) +
				")'><input type='radio' name='screenMode' value='" + String(i) +
				"' " + (state_->getScreenMode() == i ? "checked" : "") + "> " +
				labels[i] + "</label>";
	}
	html += "</div></div></div></div>";

	// ─── TAB: Countdown ──────────────────────────────────────────
	html += "<div id='tab_cd' class='tab-content'><div class='grid'>";
	html += "<div class='section'><h3>" + String(kCdTitle) + "</h3>";
	html +=
		"<label class='chk'><input type='checkbox' name='showSecCD' "
		"value='1' " +
		String(state_->isShowSecondsCountdownEnabled() ? "checked" : "") +
		"> " + kCdShowSec + "</label>";
	html +=
		"<label class='chk'><input type='checkbox' name='showMessage' "
		"value='1' " +
		String(state_->isShowMessageEnabled() ? "checked" : "") + "> " +
		kCdShowMsg + "</label>";
	html +=
		"<label class='chk'><input type='checkbox' name='rainbowState' "
		"value='1' " +
		String(state_->isRainbowEnabled() ? "checked" : "") + "> " +
		kCdRainbow + "</label>";
	html += "<label>" + String(kCdTarget) +
			"</label><input type='datetime-local' name='datetime' value='" +
			String(dtStr) + "' required>";
	html += "<label>" + String(kCdMessage) +
			"</label><input type='text' name='customText' value='" +
			state_->getCustomText() + "'>";
	html += "<label>" + String(kCdTextSpeed) + "</label>" +
			getSlider("textSpeed", 1, 10, state_->getTextSpeed());
	html += "<label>" + String(kCdRainbowSpeed) + "</label>" +
			getSlider("rainbowSpeed", 1, 10, state_->getRainbowSpeed());
	html += "</div>";
	html += "<div class='section'><h3>" + String(kCdColorTitle) + "</h3>";
	html += "<label>" + String(kCdDaysColor) + "</label><input type='color' name='cdDaysClr' value='" + state_->getCdDaysColor() + "'>";
	html += "<label>" + String(kCdTimeColor) + "</label><input type='color' name='cdTimeClr' value='" + state_->getCdTimeColor() + "'>";
	html += "</div></div></div>";

	// ─── TAB: Sensor ─────────────────────────────────────────────
	html += "<div id='tab_sensor' class='tab-content'><div class='grid'>";
	html += "<div class='section'><h3>" + String(kSenTitle) + "</h3>";
	html +=
		"<label class='chk'><input type='checkbox' name='sensorBg' value='1' " +
		String(state_->isSensorBgEnabled() ? "checked" : "") + "> " + kSenBg +
		"</label>";
	html += "</div></div></div>";

	// ─── TAB: DateTime ───────────────────────────────────────────
	html += "<div id='tab_datetime' class='tab-content'><div class='grid'>";
	html += "<div class='section'><h3>" + String(kDtTitle) + "</h3>";
	html +=
		"<label class='chk'><input type='checkbox' name='showYear' value='1' " +
		String(state_->isShowYearEnabled() ? "checked" : "") + "> " +
		kDtShowYear + "</label>";
	html +=
		"<label class='chk'><input type='checkbox' name='shortYear' "
		"value='1' " +
		String(state_->isShortYearEnabled() ? "checked" : "") + "> " +
		kDtShortYear + "</label>";
	html +=
		"<label class='chk'><input type='checkbox' name='showSecDT' "
		"value='1' " +
		String(state_->isShowSecondsDateTimeEnabled() ? "checked" : "") + "> " +
		kDtShowSec + "</label>";
	html +=
		"<label>" + String(kDtSeparator) + "</label><select name='dateSep'>";
	html += "<option value='0' " +
			String(state_->getDateSeparator() == 0 ? "selected" : "") + ">" +
			kDtSepSlash + "</option>";
	html += "<option value='1' " +
			String(state_->getDateSeparator() == 1 ? "selected" : "") + ">" +
			kDtSepDot + "</option>";
	html += "<option value='2' " +
			String(state_->getDateSeparator() == 2 ? "selected" : "") + ">" +
			kDtSepDash + "</option>";
	html += "</select></div></div></div>";

	// ─── TAB: Text ───────────────────────────────────────────────
	html += "<div id='tab_text' class='tab-content'><div class='grid'>";
	html += "<div class='section'><h3>" + String(kTxtTitle) + "</h3>";
	html +=
		"<label class='chk'><input type='checkbox' name='txtScroll' "
		"value='1' " +
		String(state_->isTextPanelScrollEnabled() ? "checked" : "") + "> " +
		kTxtScroll + "</label>";
	html += "<label>" + String(kTxtContent) +
			"</label><input type='text' name='txtContent' value='" +
			state_->getTextPanelContent() + "'>";
	html += "<label>" + String(kTxtSpeed) + "</label>" +
			getSlider("txtSpeed", 1, 10, state_->getTextPanelSpeed());
	html += "<label>" + String(kTxtSize) + "</label>" +
			getSlider("txtSize", 1, 4, state_->getTextPanelSize());
	html += "<label>" + String(kTxtColor) + "</label><input type='color' name='txtColor' value='" + state_->getTextPanelColor() + "'>";
	html += "</div></div></div>";

	// ─── TAB: Auto ───────────────────────────────────────────────
	html += "<div id='tab_auto' class='tab-content'><div class='grid'>";
	html += "<div class='section'><h3>" + String(kAutoTitle) + "</h3>";
	html +=
		"<label class='chk'><input type='checkbox' name='autoCycle' "
		"value='1' " +
		String(state_->isAutoCycleEnabled() ? "checked" : "") + "> " +
		kAutoEnable + "</label>";
	html += "<span class='note'>" + String(kAutoNote) + "</span>";
	html += "<label>" + String(kAutoSeqLabel) +
			"</label><input type='text' name='cycleSeq' value='" +
			state_->getCycleSequence() + "' placeholder='0,1,2,3'>";
	html += "<h3>" + String(kAutoTimersTitle) + "</h3>";
	html += "<label>" + String(kScrCountdown) + ":</label>" +
			getSlider("timer0", 0, 120, state_->getTimer0_Countdown());
	html += "<label>" + String(kScrSensor) + ":</label>" +
			getSlider("timer1", 0, 120, state_->getTimer1_Sensor());
	html += "<label>" + String(kScrDateTime) + ":</label>" +
			getSlider("timer2", 0, 120, state_->getTimer2_DateTime());
	html += "<label>" + String(kScrText) + ":</label>" +
			getSlider("timer3", 0, 120, state_->getTimer3_Text());
	html += "<label>" + String(kScrSpinner) + ":</label>" +
			getSlider("timer4", 0, 120, state_->getTimer4_Spinner());
	html += "</div>";

	html += "<div class='section'><h3>" + String(kFwTitle) + "</h3>";
	html += "<span class='note'>" + String(kFwNote) + "</span>";
	html += "<label>" + String(kFwLabel) +
			"</label><input type='text' name='fwScheds' value='" +
			state_->getFwSchedules() + "' placeholder='10:35,5;08:40,10'>";
	html +=
		"<button type='button' "
		"onclick='fetch(\"/test-fireworks\",{method:\"POST\"})' "
		"class='btn-small' "
		"style='background:#f39c12;margin-top:12px;width:100%;'>" +
		String(kFwTestBtn) + "</button>";
	html += "</div></div></div>";

	// ─── TAB: Spinner ───────────────────────────────────────────
	html += "<div id='tab_spinner' class='tab-content'><div class='grid'>";
	html += "<div class='section'><h3>" + String(kSpinTitle) + "</h3>";
	html += "<label>" + String(kSpinMin) + "</label><input type='number' name='spinMin' value='" + String(state_->getSpinRangeMin()) + "' min='0' max='9999'>";
	html += "<label>" + String(kSpinMax) + "</label><input type='number' name='spinMax' value='" + String(state_->getSpinRangeMax()) + "' min='1' max='9999'>";
	html += "<label>" + String(kSpinDur) + "</label>" + getSlider("spinDur", 1, 30, state_->getSpinDuration());
	html += "</div>";

	html += "<div class='section'><h3>Điều khiển</h3>";
	html += "<button type='button' id='btnDoSpin' onclick='doSpin(this)' class='btn-small' style='background:#28a745;width:100%;margin-bottom:8px;'>" + String(kSpinBtn) + "</button>";
	html += "<button type='button' id='btnSpinReset' onclick='doSpinReset(this)' class='btn-small' style='background:#dc3545;width:100%;'>" + String(kSpinReset) + "</button>";
	html += "</div></div></div>";

	// ─── TAB: OTA ────────────────────────────────────────────────
	html += "<div id='tab_ota' class='tab-content'><div class='grid'>";
	html += "<div class='section'><h3>" + String(kOtaTitle) + "</h3>";
	html += "<span class='note'>" + String(kOtaNote) + "</span>";
	html +=
		"<div "
		"style='display:flex;flex-direction:column;gap:12px;margin-top:16px;'>";
	html +=
		"<input type='file' id='firmwareInput' accept='.bin' "
		"style='padding:10px;border:1px dashed #0056b3;border-radius:8px;'>";
	html +=
		"<button type='button' onclick='uploadFirmware()' class='btn-small' "
		"style='background:#28a745;width:fit-content;'>" +
		String(kOtaBtn) + "</button>";
	html += "<progress id='uploadProgress' max='100' value='0'></progress>";
	html +=
		"<span id='statusMsg' "
		"style='font-weight:bold;color:#0056b3;'></span></div>";

	html += R"rawjavascript(
<script>
function uploadFirmware(){
var f=document.getElementById('firmwareInput');
if(!f.files.length){alert('Chọn file .bin!');return;}
var d=new FormData();d.append('update',f.files[0]);
var x=new XMLHttpRequest(),p=document.getElementById('uploadProgress'),s=document.getElementById('statusMsg');
p.style.display='block';
x.upload.addEventListener('progress',function(e){var pct=Math.round(e.loaded/e.total*100);p.value=pct;s.innerText=')rawjavascript";
	html += kOtaUploading;
	html += R"rawjavascript('+pct+'%';});
x.addEventListener('load',function(){if(x.status===200){s.innerText=')rawjavascript";
	html += kOtaSuccess;
	html +=
		R"rawjavascript(';s.style.color='green';setTimeout(function(){window.location.reload();},8000);}else{s.innerText='Error '+x.status;s.style.color='red';}});
x.addEventListener('error',function(){s.innerText=')rawjavascript";
	html += kOtaError;
	html += R"rawjavascript(';s.style.color='red';});
x.open('POST','/update');x.send(d);}
</script>
)rawjavascript";

	html += "</div></div></div>";
	html += "</form></div></div></body></html>";
	request->send(200, "text/html", html);
}

void WebConfigServer::handleSave(AsyncWebServerRequest *request) {
	auto hasArg = [&](const String &n) {
		return request->hasParam(n, true) || request->hasParam(n, false);
	};
	auto arg = [&](const String &n) {
		if (request->hasParam(n, true))
			return request->getParam(n, true)->value();
		if (request->hasParam(n, false))
			return request->getParam(n, false)->value();
		return String("");
	};

	state_->lock();

	if (hasArg("datetime")) {
		String dt = arg("datetime");
		int yr, mo, da, hr, mn;
		if (sscanf(dt.c_str(), "%d-%d-%dT%d:%d", &yr, &mo, &da, &hr, &mn) ==
			5) {
			struct tm tm_target = {0};
			tm_target.tm_year	= yr - 1900;
			tm_target.tm_mon	= mo - 1;
			tm_target.tm_mday	= da;
			tm_target.tm_hour	= hr;
			tm_target.tm_min	= mn;
			tm_target.tm_sec	= 0;
			tm_target.tm_isdst	= -1;
			time_t newEpoch		= mktime(&tm_target);
			if (newEpoch != -1) state_->setTargetEpoch(newEpoch);
		}
	}

	state_->setLedEnabled(hasArg("ledState") && arg("ledState") == "1");
	state_->setSplashEnabled(hasArg("splashState") &&
							 arg("splashState") == "1");
	splash_->setEnabled(state_->isSplashEnabled());
	if (hasArg("splashText")) state_->setSplashText(arg("splashText"));

	if (hasArg("brightness")) {
		state_->setBrightness(arg("brightness").toInt());
		display_->setBrightness(state_->getBrightness());
	}

	if (hasArg("screenMode")) state_->setScreenMode(arg("screenMode").toInt());

	if (hasArg("apSSID")) state_->setApSSID(arg("apSSID"));
	if (hasArg("apPass")) state_->setApPass(arg("apPass"));
	if (hasArg("staSSID")) state_->setStaSSID(arg("staSSID"));
	if (hasArg("staPass")) state_->setStaPass(arg("staPass"));

	state_->setShowYearEnabled(hasArg("showYear") && arg("showYear") == "1");
	state_->setShortYearEnabled(hasArg("shortYear") && arg("shortYear") == "1");
	if (hasArg("dateSep")) state_->setDateSeparator(arg("dateSep").toInt());
	state_->setShowSecondsDateTimeEnabled(hasArg("showSecDT") &&
										  arg("showSecDT") == "1");

	if (hasArg("customText")) state_->setCustomText(arg("customText"));

	state_->setShowSecondsCountdownEnabled(hasArg("showSecCD") &&
										   arg("showSecCD") == "1");
	state_->setShowMessageEnabled(hasArg("showMessage") &&
								  arg("showMessage") == "1");
	state_->setRainbowEnabled(hasArg("rainbowState") &&
							  arg("rainbowState") == "1");

	if (hasArg("cdDaysClr")) state_->setCdDaysColor(arg("cdDaysClr"));
	if (hasArg("cdTimeClr")) state_->setCdTimeColor(arg("cdTimeClr"));

	if (hasArg("textSpeed")) state_->setTextSpeed(arg("textSpeed").toInt());
	if (hasArg("rainbowSpeed"))
		state_->setRainbowSpeed(arg("rainbowSpeed").toInt());

	state_->setSensorBgEnabled(hasArg("sensorBg") && arg("sensorBg") == "1");
	state_->setWifiIconEnabled(hasArg("wifiIcon") && arg("wifiIcon") == "1");

	if (hasArg("txtContent")) state_->setTextPanelContent(arg("txtContent"));
	if (hasArg("txtSpeed")) state_->setTextPanelSpeed(arg("txtSpeed").toInt());
	if (hasArg("txtSize")) state_->setTextPanelSize(arg("txtSize").toInt());
	if (hasArg("txtColor")) state_->setTextPanelColor(arg("txtColor"));
	state_->setTextPanelScrollEnabled(hasArg("txtScroll") &&
									  arg("txtScroll") == "1");

	state_->setAutoCycleEnabled(hasArg("autoCycle") && arg("autoCycle") == "1");
	if (hasArg("timer0")) state_->setTimer0_Countdown(arg("timer0").toInt());
	if (hasArg("timer1")) state_->setTimer1_Sensor(arg("timer1").toInt());
	if (hasArg("timer2")) state_->setTimer2_DateTime(arg("timer2").toInt());
	if (hasArg("timer3")) state_->setTimer3_Text(arg("timer3").toInt());
	if (hasArg("cycleSeq")) state_->setCycleSequence(arg("cycleSeq"));
	if (hasArg("fwScheds")) state_->setFwSchedules(arg("fwScheds"));

	// Spinner settings
	if (hasArg("spinMin")) state_->setSpinRangeMin(arg("spinMin").toInt());
	if (hasArg("spinMax")) state_->setSpinRangeMax(arg("spinMax").toInt());
	if (hasArg("spinDur")) state_->setSpinDuration(arg("spinDur").toInt());
	if (hasArg("timer4")) state_->setTimer4_Spinner(arg("timer4").toInt());

	state_->unlock();

	String activeTab = "sys";
	if (hasArg("activeTab")) activeTab = arg("activeTab");

	AsyncWebServerResponse *response =
		request->beginResponse(302, "text/plain", "Saved");
	response->addHeader("Location", "/?tab=" + activeTab);
	request->send(response);
}

void WebConfigServer::handleRestart(AsyncWebServerRequest *request) {
	using namespace WebUI;
	request->send(200, "text/html",
				  "<html><head><meta charset='utf-8'><meta name='viewport' "
				  "content='width=device-width, initial-scale=1'>"
				  "</head><body "
				  "style='background:#f0f2f5;color:#1c1e21;text-align:center;"
				  "font-family:Arial;padding-top:50px;'>"
				  "<h2>" +
					  String(kRestartMsg) +
					  "</h2>"
					  "<p>" +
					  String(kRestartNote) +
					  "</p>"
					  "</body></html>");
	shouldReboot_ = true;
}

void WebConfigServer::handleShowSplash(AsyncWebServerRequest *request) {
	using namespace WebUI;
	request->send(200, "text/html",
				  "<html><head><meta charset='utf-8'><meta name='viewport' "
				  "content='width=device-width, initial-scale=1'>"
				  "<meta http-equiv='refresh' content='11;url=/'></head>"
				  "<body "
				  "style='background:#f0f2f5;color:#1c1e21;text-align:center;"
				  "font-family:Arial;padding-top:50px;'>"
				  "<h2>" +
					  String(kSplashMsg) +
					  "</h2>"
					  "<p>" +
					  String(kSplashNote) +
					  "</p>"
					  "</body></html>");
	state_->requestSplashPlay();
}

void WebConfigServer::handleTestFireworks(AsyncWebServerRequest *request) {
	fireworks_->trigger(5);
	request->send(200, "text/plain", "OK");
}
