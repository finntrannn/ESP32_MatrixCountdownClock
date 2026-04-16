#pragma once

const char HTML_UI_STATIC[] PROGMEM = R"rawhtml(
<!DOCTYPE html><html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no'>
<title>Bảng Điều Khiển Đồng Hồ</title>
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

.main-content{flex:1;padding:24px;overflow-y:auto;display:flex;flex-direction:column;position:relative;}
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

.hamburger{display:none;background:#0056b3;color:#fff;border:none;border-radius:8px;width:40px;height:40px;flex-shrink:0;font-size:22px;cursor:pointer;box-shadow:0 2px 6px rgba(0,0,0,.15);line-height:1;transition:opacity .2s;margin-bottom:10px;}
.overlay{display:none;position:fixed;inset:0;background:rgba(0,0,0,.35);z-index:15;}

#toast{position:fixed;bottom:20px;right:20px;background:#28a745;color:white;padding:15px 25px;border-radius:8px;box-shadow:0 4px 12px rgba(0,0,0,.15);font-weight:bold;z-index:9999;transition:0.3s;opacity:0;transform:translateY(20px);pointer-events:none;}
#toast.show{opacity:1;transform:translateY(0);}
#toast.error{background:#dc3545;}

@media(max-width:768px){
  .hamburger{display:block;}
  .sidebar{position:fixed;left:-300px;top:0;bottom:0;width:260px;transition:left .25s ease;z-index:30;}
  .sidebar.open{left:0;}
  .sidebar.open~.overlay+.main-content .hamburger,.hamburger.hidden{opacity:0;pointer-events:none;}
  .overlay.show{display:block;}
  .main-content{padding:14px;}
  .top-actions{flex-wrap:wrap;gap:6px;}
  .top-actions>button{flex:1;min-width:0;}
  .btn-small{width:100%;padding:10px 6px;font-size:12px;}
}
</style>
<script>
let stateData = {};

function initApp() {
  const urlParams = new URLSearchParams(window.location.search);
  const tab = urlParams.get('tab') || 'sys';
  opentabByString(tab);

  fetch('/api/config').then(r => r.json()).then(data => {
    stateData = data;
    populateForm();
  }).catch(e => showToast("Lỗi tải cấu hình (Mất kết nối)", true));
}

function showToast(msg, isError) {
  var t = document.getElementById("toast");
  t.innerText = msg;
  t.className = isError ? "error show" : "show";
  setTimeout(()=>t.className="", 3000);
}

function populateForm() {
  const d = stateData;
  document.querySelector('[name=staSSID]').value = d.staSSID || '';
  document.querySelector('[name=staPass]').value = d.staPass || '';
  document.querySelector('[name=apSSID]').value = d.apSSID || '';
  document.querySelector('[name=apPass]').value = d.apPass || '';

  document.querySelector('[name=ledState]').checked = d.ledState;
  document.querySelector('[name=splashState]').checked = d.splashState;
  document.querySelector('[name=splashText]').value = d.splashText || '';
  document.querySelector('[name=wifiIcon]').checked = d.wifiIcon;
  syncSlider('brightness', d.brightness);
  
  selRadio('screenMode', d.screenMode);

  document.querySelector('[name=showSecCD]').checked = d.showSecCD;
  document.querySelector('[name=showMessage]').checked = d.showMessage;
  document.querySelector('[name=rainbowState]').checked = d.rainbowState;
  
  if (d.targetEpoch) {
      const date = new Date(d.targetEpoch * 1000);
      const iso = new Date(date.getTime() - (date.getTimezoneOffset() * 60000)).toISOString().slice(0, 16);
      document.querySelector('[name=datetime]').value = iso;
  }
  document.querySelector('[name=customText]').value = d.customText || '';
  syncSlider('textSpeed', d.textSpeed);
  syncSlider('rainbowSpeed', d.rainbowSpeed);
  document.querySelector('[name=cdDaysClr]').value = d.cdDaysColor || '#ffff00';
  document.querySelector('[name=cdTimeClr]').value = d.cdTimeColor || '#00ffff';

  document.querySelector('[name=sensorBg]').checked = d.sensorBg;
  document.querySelector('[name=openMeteoEn]').checked = d.openMeteoEn;
  document.querySelector('[name=weaLat]').value = d.weaLat !== undefined ? d.weaLat : 21.0285;
  document.querySelector('[name=weaLon]').value = d.weaLon !== undefined ? d.weaLon : 105.8542;

  document.querySelector('[name=showYear]').checked = d.showYear;
  document.querySelector('[name=shortYear]').checked = d.shortYear;
  document.querySelector('[name=showSecDT]').checked = d.showSecDT;
  document.querySelector('[name=dateSep]').value = d.dateSep || '0';

  document.querySelector('[name=txtScroll]').checked = d.txtScroll;
  document.querySelector('[name=txtContent]').value = d.txtContent || '';
  syncSlider('txtSpeed', d.txtSpeed);
  syncSlider('txtSize', d.txtSize);
  document.querySelector('[name=txtColor]').value = d.txtColor || '#00FF00';

  document.querySelector('[name=autoCycle]').checked = d.autoCycle;
  document.querySelector('[name=cycleSeq]').value = d.cycleSeq || '';
  syncSlider('timer0', d.timer0);
  syncSlider('timer1', d.timer1);
  syncSlider('timer2', d.timer2);
  syncSlider('timer3', d.timer3);
  syncSlider('timer4', d.timer4);

  document.querySelector('[name=fwScheds]').value = d.fwScheds || '';

  document.querySelector('[name=spinMin]').value = d.spinMin;
  document.querySelector('[name=spinMax]').value = d.spinMax;
  syncSlider('spinDur', d.spinDur);
}

function syncSlider(name, val) {
  var r = document.getElementById('r_' + name);
  var n = document.getElementById('n_' + name);
  if (r) r.value = val;
  if (n) n.value = val;
}

function saveConfig(e) {
  e.preventDefault();
  const formData = new FormData(document.getElementById('mForm'));
  
  fetch('/save', {
    method: 'POST',
    body: new URLSearchParams(formData)
  })
  .then(r => r.json())
  .then(data => {
    if (data.status === 'ok') showToast("Đã lưu cấu hình", false);
    else showToast("Lỗi lưu cấu hình", true);
  }).catch(e => showToast("Mất kết nối", true));
}

function reqCmd(url) {
  fetch(url, {method:'POST'}).then(r=>{
      showToast("Đã gửi lệnh", false);
  });
}

function opentabByString(t) {
  let parent = "";
  if (["cd","sensor","datetime","text","spinner","scr_gen"].includes(t)) parent = "dd_disp";
  opentab(t, parent);
}

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
  history.replaceState(null, '', '?tab=' + t);
  if(window.innerWidth<=768) closeMenu();
}
function selRadio(name,val){
  var l=document.getElementsByTagName('label');
  for(var i=0;i<l.length;i++) if(l[i].getAttribute('data-rn')===name)l[i].className='';
  document.getElementById('lbl_'+name+'_'+val).className='selected';
  
  var rad = document.querySelector('input[name="'+name+'"][value="'+val+'"]');
  if(rad) rad.checked = true;
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
function uploadFirmware(){
  var f=document.getElementById('firmwareInput');
  if(!f.files.length){alert('Chọn file .bin!');return;}
  var d=new FormData();d.append('update',f.files[0]);
  var x=new XMLHttpRequest(),p=document.getElementById('uploadProgress'),s=document.getElementById('statusMsg');
  p.style.display='block';
  x.upload.addEventListener('progress',function(e){var pct=Math.round(e.loaded/e.total*100);p.value=pct;s.innerText='Đang tải lên: '+pct+'%';});
  x.addEventListener('load',function(){
    if(x.status===200){s.innerText='Hoàn tất! Đang khởi động lại...';s.style.color='green';setTimeout(function(){window.location.reload();},8000);}
    else{s.innerText='Lỗi: '+x.status;s.style.color='red';}
  });
  x.addEventListener('error',function(){s.innerText='Lỗi mất kết nối!';s.style.color='red';});
  x.open('POST','/update');x.send(d);
}

window.onload = initApp;
</script>
</head><body>
<div id="toast"></div>
<div class='overlay' id='overlay' onclick='closeMenu()'></div>
<div class='sidebar' id='sidebar'>
  <div class='sidebar-header'><h2>Bảng Điều Khiển</h2></div>
  <button type='button' class='dropdown-btn' onclick='toggleDropdown(this)' id='dd_disp'>Màn hình & hiển thị <span class='dropdown-icon'>&#9660;</span></button>
  <div class='dropdown-container'>
    <button type='button' id='btn_scr_gen' class='tab-btn' onclick='opentab("scr_gen","dd_disp")'>Cấu hình chung</button>
    <button type='button' id='btn_cd' class='tab-btn' onclick='opentab("cd","dd_disp")'>MH Đếm ngược</button>
    <button type='button' id='btn_sensor' class='tab-btn' onclick='opentab("sensor","dd_disp")'>MH Nhiệt độ & độ ẩm</button>
    <button type='button' id='btn_datetime' class='tab-btn' onclick='opentab("datetime","dd_disp")'>MH Ngày & giờ</button>
    <button type='button' id='btn_text' class='tab-btn' onclick='opentab("text","dd_disp")'>MH Văn bản tự do</button>
    <button type='button' id='btn_spinner' class='tab-btn' onclick='opentab("spinner","dd_disp")'>MH Quay số</button>
  </div>
  <button type='button' id='btn_sys' class='flat-btn' onclick='opentab("sys")'>Cài đặt WiFi</button>
  <button type='button' id='btn_auto' class='flat-btn' onclick='opentab("auto")'>Tự động & lịch</button>
  <button type='button' id='btn_ota' class='flat-btn' onclick='opentab("ota")'>Cập nhật phần mềm</button>
</div>

<div class='main-content'>
  <button class='hamburger' id='hbtn' onclick='toggleMenu()'>&#9776;</button>
  <div class='top-actions'>
    <button type='submit' form='mForm' class='btn-small' style='background:#0056b3;'>Lưu Cấu Hình</button>
    <button type='button' onclick='reqCmd("/showsplash")' class='btn-small' style='background:#f39c12;'>Hiệu ứng Splash</button>
    <button type='button' onclick='if(confirm("Khởi động lại ESP32?")) reqCmd("/restart")' class='btn-small' style='background:#dc3545;'>Khởi Động Lại</button>
  </div>

  <form id='mForm' onsubmit='saveConfig(event)'>
    <input type='hidden' id='activeTabInput' name='activeTab' value='sys'>

    <div id='tab_sys' class='tab-content'><div class='grid'>
      <div class='section'><h3>Cấu hình WiFi tĩnh cho ESP32</h3>
        <label>Tên WiFi:</label><input type='text' name='staSSID' required>
        <label>Mật khẩu:</label><input type='text' name='staPass'>
      </div>
      <div class='section'><h3>Cấu thiết bị phát (Access Point)</h3>
        <label>Tên WiFi:</label><input type='text' name='apSSID' required>
        <label>Mật khẩu:</label><input type='text' name='apPass' required>
      </div>
    </div></div>

    <div id='tab_scr_gen' class='tab-content'><div class='grid'>
      <div class='section'><h3>Cấu hình chung</h3>
        <label class='chk'><input type='checkbox' name='ledState' value='1'> Bật màn hình LED</label>
        <label class='chk'><input type='checkbox' name='splashState' value='1'> Hiện splash screen lúc khởi động</label>
        <label>Nội dung Splash Screen (tối đa ~10 ký tự):</label><input type='text' name='splashText' maxlength='20'>
        <label class='chk'><input type='checkbox' name='wifiIcon' value='1'> Hiện biểu tượng WiFi</label>
        <label>Độ sáng màn hình (1 - 255):</label>
        <div class='sg'><input type='range' id='r_brightness' name='brightness' min='1' max='255' oninput='syncS("n_brightness", this.value)'><input type='number' id='n_brightness' min='1' max='255' oninput='syncS("r_brightness", this.value)'></div>
      </div>
      <div class='section'><h3>Chế độ màn hình hiện tại</h3>
        <span class='note'>Chỉ áp dụng khi "Tự động chuyển màn hình" bị TẮT.</span>
        <div class='radio-group'>
          <label data-rn='screenMode' id='lbl_screenMode_0' onclick='selRadio("screenMode",0)'><input type='radio' name='screenMode' value='0'> Đếm ngược</label>
          <label data-rn='screenMode' id='lbl_screenMode_1' onclick='selRadio("screenMode",1)'><input type='radio' name='screenMode' value='1'> Đo nhiệt độ & độ ẩm</label>
          <label data-rn='screenMode' id='lbl_screenMode_2' onclick='selRadio("screenMode",2)'><input type='radio' name='screenMode' value='2'> Ngày & giờ</label>
          <label data-rn='screenMode' id='lbl_screenMode_3' onclick='selRadio("screenMode",3)'><input type='radio' name='screenMode' value='3'> Văn bản tự do</label>
          <label data-rn='screenMode' id='lbl_screenMode_4' onclick='selRadio("screenMode",4)'><input type='radio' name='screenMode' value='4'> Quay số (Spinner)</label>
        </div>
      </div>
    </div></div>

    <div id='tab_cd' class='tab-content'><div class='grid'>
      <div class='section'><h3>Cấu hình đếm ngược</h3>
        <label class='chk'><input type='checkbox' name='showSecCD' value='1'> Hiện số giây đang trôi</label>
        <label class='chk'><input type='checkbox' name='showMessage' value='1'> Hiện lời động viên</label>
        <label class='chk'><input type='checkbox' name='rainbowState' value='1'> Bật ruy băng cầu vồng</label>
        <label>Ngày/giờ mục tiêu (Target Event):</label><input type='datetime-local' name='datetime' required>
        <label>Lời nhắn động viên:</label><input type='text' name='customText'>
        <label>Tốc độ cuộn chữ (1 - 10):</label>
        <div class='sg'><input type='range' id='r_textSpeed' name='textSpeed' min='1' max='10' oninput='syncS("n_textSpeed", this.value)'><input type='number' id='n_textSpeed' min='1' max='10' oninput='syncS("r_textSpeed", this.value)'></div>
        <label>Vận tốc cuộn cầu vồng (1 - 10):</label>
        <div class='sg'><input type='range' id='r_rainbowSpeed' name='rainbowSpeed' min='1' max='10' oninput='syncS("n_rainbowSpeed", this.value)'><input type='number' id='n_rainbowSpeed' min='1' max='10' oninput='syncS("r_rainbowSpeed", this.value)'></div>
      </div>
      <div class='section'><h3>Màu sắc</h3>
        <label>Màu chữ "CÒN... NGÀY":</label><input type='color' name='cdDaysClr'>
        <label>Màu khối thời lượng (h:m:s):</label><input type='color' name='cdTimeClr'>
      </div>
    </div></div>

    <div id='tab_sensor' class='tab-content'><div class='grid'>
      <div class='section'><h3>Màn hình cảm biến nhiệt/ẩm</h3>
        <label class='chk'><input type='checkbox' name='sensorBg' value='1'> Background 3D Gradient sinh động</label>
        <h3 style='margin-top:16px; border-bottom:none; margin-bottom:5px;'>Thời Tiết Thực Tế (Mạng)</h3>
        <label class='chk'><input type='checkbox' name='openMeteoEn' value='1'> Bật thời tiết thực tế (Open-Meteo)</label>
        <span class='note'>Cần 15p để đồng bộ dữ liệu mỗi lần bật máy.</span>
        <label>Vĩ độ (Latitude):</label><input type='number' step='any' name='weaLat' required>
        <label>Kinh độ (Longitude):</label><input type='number' step='any' name='weaLon' required>
      </div>
    </div></div>

    <div id='tab_datetime' class='tab-content'><div class='grid'>
      <div class='section'><h3>Màn hình Ngày & Giờ NTP</h3>
        <label class='chk'><input type='checkbox' name='showYear' value='1'> Hiện con số năm</label>
        <label class='chk'><input type='checkbox' name='shortYear' value='1'> Viết gọn năm thành 2 chữ số (VD: 24)</label>
        <label class='chk'><input type='checkbox' name='showSecDT' value='1'> Chạy từng giây thời gian thực</label>
        <label>Dấu ngăn cách phân khối ngày tháng:</label>
        <select name='dateSep'>
          <option value='0'>Phân cách "/" (VD: 20/11)</option>
          <option value='1'>Phân cách "." (VD: 20.11)</option>
          <option value='2'>Phân cách "-" (VD: 20-11)</option>
        </select>
      </div>
    </div></div>

    <div id='tab_text' class='tab-content'><div class='grid'>
      <div class='section'><h3>Màn hình văn bản linh hoạt</h3>
        <label class='chk'><input type='checkbox' name='txtScroll' value='1'> Chữ cuộn tròn ngang qua màn</label>
        <label>Nội dung muốn hiển thị:</label><input type='text' name='txtContent'>
        <label>Tốc độ (1-10):</label>
        <div class='sg'><input type='range' id='r_txtSpeed' name='txtSpeed' min='1' max='10' oninput='syncS("n_txtSpeed", this.value)'><input type='number' id='n_txtSpeed' min='1' max='10' oninput='syncS("r_txtSpeed", this.value)'></div>
        <label>Size chữ (1-4):</label>
        <div class='sg'><input type='range' id='r_txtSize' name='txtSize' min='1' max='4' oninput='syncS("n_txtSize", this.value)'><input type='number' id='n_txtSize' min='1' max='4' oninput='syncS("r_txtSize", this.value)'></div>
        <label>Màu văn bản (Hex Color):</label><input type='color' name='txtColor'>
      </div>
    </div></div>

    <div id='tab_auto' class='tab-content'><div class='grid'>
      <div class='section'><h3>Hiệu ứng vòng lặp (Auto-Cycle)</h3>
        <label class='chk'><input type='checkbox' name='autoCycle' value='1'> Tự động chuyển qua lại giữa các màn hình</label>
        <span class='note'>0: Đếm ngược, 1: Cảm biến, 2: Lịch, 3: Văn bản, 4: Lô tô.<br>VD: "0,2,0" để chỉ xoay vòng Đếm ngược & Lịch.</span>
        <label>Chuỗi trình tự lặp (phân tách qua dấu phẩy):</label><input type='text' name='cycleSeq' placeholder='0,1,2,3,4'>
        <h3 style='margin-top:20px;'>Thời gian chốt lại (giây/màn hình)</h3>
        <label>Đếm ngược:</label><div class='sg'><input type='range' id='r_timer0' name='timer0' min='0' max='120' oninput='syncS("n_timer0", this.value)'><input type='number' id='n_timer0' min='0' max='120' oninput='syncS("r_timer0", this.value)'></div>
        <label>Cảm biến:</label><div class='sg'><input type='range' id='r_timer1' name='timer1' min='0' max='120' oninput='syncS("n_timer1", this.value)'><input type='number' id='n_timer1' min='0' max='120' oninput='syncS("r_timer1", this.value)'></div>
        <label>Ngày giờ:</label><div class='sg'><input type='range' id='r_timer2' name='timer2' min='0' max='120' oninput='syncS("n_timer2", this.value)'><input type='number' id='n_timer2' min='0' max='120' oninput='syncS("r_timer2", this.value)'></div>
        <label>Văn bản:</label><div class='sg'><input type='range' id='r_timer3' name='timer3' min='0' max='120' oninput='syncS("n_timer3", this.value)'><input type='number' id='n_timer3' min='0' max='120' oninput='syncS("r_timer3", this.value)'></div>
        <label>Quay Lô tô:</label><div class='sg'><input type='range' id='r_timer4' name='timer4' min='0' max='120' oninput='syncS("n_timer4", this.value)'><input type='number' id='n_timer4' min='0' max='120' oninput='syncS("r_timer4", this.value)'></div>
      </div>
      <div class='section'><h3>Lịch kịch bản Pháo hoa</h3>
        <span class='note'>Cài báo thức châm pháo thực tế. Mã kịch bản: HH:MM,SốS;<br>VD: 10:35,5;08:40,10</span>
        <label>Thiết đặt chuỗi kịch bản pháo hoa:</label><input type='text' name='fwScheds'>
        <button type='button' onclick='reqCmd("/test-fireworks")' class='btn-small' style='background:#f39c12;width:100%;margin-top:10px;'>Bắn pháo hoa thử (5s)</button>
      </div>
    </div></div>

    <div id='tab_spinner' class='tab-content'><div class='grid'>
      <div class='section'><h3>Màn hình quay số lô tô</h3>
        <label>Min (Bắt đầu):</label><input type='number' name='spinMin' min='0' max='9999'>
        <label>Max (Kết thúc):</label><input type='number' name='spinMax' min='1' max='9999'>
        <label>Thời lượng chạy số trớn (giây):</label><div class='sg'><input type='range' id='r_spinDur' name='spinDur' min='1' max='30' oninput='syncS("n_spinDur", this.value)'><input type='number' id='n_spinDur' min='1' max='30' oninput='syncS("r_spinDur", this.value)'></div>
      </div>
      <div class='section'><h3>Điều khiển thủ công</h3>
        <button type='button' onclick='doSpin(this)' class='btn-small' style='background:#28a745;width:100%;margin-bottom:10px;'>Mở Vòng Quay (Trigger)</button>
        <button type='button' onclick='doSpinReset(this)' class='btn-small' style='background:#dc3545;width:100%;'>Cho vòng lặp mới (Reset Range)</button>
      </div>
    </div></div>

    <div id='tab_ota' class='tab-content'><div class='grid'>
      <div class='section'><h3>Nâng cấp Firmware không dây</h3>
        <span class='note'>Update Firmware trực tiếp qua BIN. Thông số bộ nhớ đệm an toàn tự giữ lại. Mạch sẽ restart sau khi load xong.</span>
        <input type='file' id='firmwareInput' accept='.bin' style='padding:10px;border:1px dashed #0056b3;border-radius:8px;'>
        <button type='button' onclick='uploadFirmware()' class='btn-small' style='background:#28a745;margin-top:10px;width:100%;'>Bắt đầu Update Cốt lõi</button>
        <progress id='uploadProgress' max='100' value='0'></progress>
        <span id='statusMsg' style='display:block;margin-top:5px;font-weight:bold;'></span>
      </div>
    </div></div>

  </form>
</div>
</body></html>
)rawhtml";
