/**
 * @file WebConfigServer.cpp
 * @brief Implementation of the web-based configuration portal.
 * 
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#include "WebConfigServer.h"

void WebConfigServer::begin(AppState& state, DisplayManager& display, SplashScreen& splash) {
    state_   = &state;
    display_ = &display;
    splash_  = &splash;

    server_.on("/",           [this]() { handleRoot(); });
    server_.on("/save",       HTTP_POST, [this]() { handleSave(); });
    server_.on("/restart",    HTTP_POST, [this]() { handleRestart(); });
    server_.on("/showsplash", HTTP_POST, [this]() { handleShowSplash(); });

    server_.begin();
}

void WebConfigServer::handleClient() {
    server_.handleClient();
}

void WebConfigServer::handleRoot() {
    time_t targetEpoch = state_->getTargetEpoch();
    struct tm* tm_targ = localtime(&targetEpoch);
    char dtStr[20];
    sprintf(dtStr, "%04d-%02d-%02dT%02d:%02d",
            tm_targ->tm_year + 1900, tm_targ->tm_mon + 1,
            tm_targ->tm_mday, tm_targ->tm_hour, tm_targ->tm_min);

    String html = R"rawhtml(
<html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1'>
<style>
body{font-family: Arial; padding: 20px; text-align:center; background:#1a1a2e; color:#eee;}
div.container{max-width: 420px; margin: auto;}
.header-row{display:flex; justify-content:space-between; align-items:center; margin-bottom:15px;}
.header-row h2{margin:0; color:#e94560; font-size:18px; text-align:left;}
.btn-group{display:flex; gap:6px; justify-content:flex-end;}
.btn-small{padding:8px 10px; font-size:12px; border-radius:4px; width:auto; border:none; cursor:pointer; color:#fff; margin:0;}
.section{background:#16213e; padding:16px; border-radius:10px; margin-bottom:14px; border:1px solid #0f3460;}
.section h3{margin:0 0 12px 0; font-size:15px; color:#e94560; text-align:left;}
input, button{display: block; width: 100%; padding: 10px; margin: 8px 0; box-sizing:border-box; border-radius: 5px; border:1px solid #333; background:#1a1a2e; color:#eee;}
input[type='range']{padding:4px 0;}
button{background-color: #28a745; color: white; border: none; font-size:15px; cursor: pointer;}
button:hover{background-color: #218838;}
label{display:block; text-align:left; font-size:13px; margin-top:6px;}
.radio-group{display:flex; gap:8px; margin:8px 0;}
.radio-group label{flex:1; display:flex; align-items:center; cursor:pointer; padding:10px; border-radius:5px; font-size:13px; margin:0;}
.chk{display:flex; align-items:center; cursor:pointer; margin:6px 0;}
.chk input{width:auto; margin:0 8px 0 0;}
</style></head><body>
<div class='container'>
<div class='header-row'>
<h2>&#9881;&#65039; C&#7845;u h&#236;nh</h2>
<div class='btn-group'>
<button type='submit' form='mainForm' class='btn-small' style='background:#28a745;'>&#128190; L&#432;u</button>
<form action='/showsplash' method='POST' style='margin:0;'><button type='submit' class='btn-small' style='background:#ffc107; color:#000;'>&#10024; Test</button></form>
<form action='/restart' method='POST' onsubmit='return confirm("X&aacute;c nh&#7853;n kh&#7903;i &#273;&#7897;ng l&#7841;i?");' style='margin:0;'><button type='submit' class='btn-small' style='background:#dc3545;'>&#128260; Reboot</button></form>
</div>
</div>
<form id='mainForm' action='/save' method='POST'>
)rawhtml";

    // ═══════════════════════════════════════════════════════════════
    // SECTION 1: Common Settings
    // ═══════════════════════════════════════════════════════════════
    html += "<div class='section'>";
    html += "<h3>&#128736; C\u00e0i \u0110\u1eb7t Chung</h3>";

    // Screen mode selector
    html += "<label>\xF0\x9F\x96\xA5\xEF\xB8\x8F Ch\u1ebf \u0111\u1ed9 m\u00e0n h\u00ecnh:</label>";
    html += "<div class='radio-group'>";
    html += "<label style='background:" + String(state_->getScreenMode() == 0 ? "#28a745" : "#0f3460") + ";'>";
    html += "<input type='radio' name='screenMode' value='0' style='width:auto; margin-right:6px;' "
            + String(state_->getScreenMode() == 0 ? "checked" : "") + "> \u23F3 \u0110\u1ebfm Ng\u01b0\u1ee3c</label>";
    html += "<label style='background:" + String(state_->getScreenMode() == 1 ? "#17a2b8" : "#0f3460") + ";'>";
    html += "<input type='radio' name='screenMode' value='1' style='width:auto; margin-right:6px;' "
            + String(state_->getScreenMode() == 1 ? "checked" : "") + "> \xF0\x9F\x8C\xA1 Nhi\u1ec7t \u0110\u1ed9</label>";
    html += "</div>";

    // LED toggle
    html += "<label class='chk'>";
    html += "<input type='checkbox' name='ledState' value='1' "
            + String(state_->isLedEnabled() ? "checked" : "") + "> B\u1eadt M\u00e0n H\u00ecnh LED</label>";

    // Splash toggle
    html += "<label class='chk'>";
    html += "<input type='checkbox' name='splashState' value='1' "
            + String(state_->isSplashEnabled() ? "checked" : "") + "> B\u1eadt Splash Screen (12A6)</label>";

    // Brightness
    html += "<label>\xF0\x9F\x94\x86 \u0110\u1ed9 s\u00e1ng (1 - 255):</label>";
    html += "<input type='range' name='brightness' min='1' max='255' value='" + String(state_->getBrightness()) + "'>";

    html += "</div>";  // end section 1

    // ═══════════════════════════════════════════════════════════════
    // SECTION 2: Countdown Settings
    // ═══════════════════════════════════════════════════════════════
    html += "<div class='section'>";
    html += "<h3>\u23F3 C\u00e0i \u0110\u1eb7t \u0110\u1ebfm Ng\u01b0\u1ee3c</h3>";

    // Target date/time
    html += "<label>\xF0\x9F\x95\x92 Ng\u00e0y/Gi\u1edd Thi:</label>";
    html += "<input type='datetime-local' name='datetime' value='" + String(dtStr) + "' required>";

    // Custom text
    html += "<label>\xF0\x9F\x92\xAC Th\u00f4ng \u0111i\u1ec7p ch\u1ea1y (kh\u00f4ng d\u1ea5u):</label>";
    html += "<input type='text' name='customText' value='" + state_->getCustomText() + "' required>";

    // Text speed
    html += "<label>T\u1ed1c \u0111\u1ed9 ch\u1eef ch\u1ea1y (1 - 10):</label>";
    html += "<input type='range' name='textSpeed' min='1' max='10' value='" + String(state_->getTextSpeed()) + "'>";

    // Show seconds toggle
    html += "<label class='chk'>";
    html += "<input type='checkbox' name='showSeconds' value='1' "
            + String(state_->isShowSecondsEnabled() ? "checked" : "") + "> \xE2\x8F\xB1 Hi\u1ec3n th\u1ecb gi\u00e2y</label>";

    // Rainbow toggle
    html += "<label class='chk'>";
    html += "<input type='checkbox' name='rainbowState' value='1' "
            + String(state_->isRainbowEnabled() ? "checked" : "") + "> \xF0\x9F\x8C\x88 Vi\u1ec1n C\u1ea7u V\u1ed3ng</label>";

    // Rainbow speed
    html += "<label>T\u1ed1c \u0111\u1ed9 vi\u1ec1n m\u00e0u (1 - 10):</label>";
    html += "<input type='range' name='rainbowSpeed' min='1' max='10' value='" + String(state_->getRainbowSpeed()) + "'>";

    html += "</div>";  // end section 2

    // ═══════════════════════════════════════════════════════════════
    // SECTION 3: Sensor Settings
    // ═══════════════════════════════════════════════════════════════
    html += "<div class='section'>";
    html += "<h3>\xF0\x9F\x8C\xA1 C\u00e0i \u0110\u1eb7t C\u1ea3m Bi\u1ebfn</h3>";

    // Background gradient toggle
    html += "<label class='chk'>";
    html += "<input type='checkbox' name='sensorBg' value='1' "
            + String(state_->isSensorBgEnabled() ? "checked" : "") + "> \xF0\x9F\x8E\xA8 N\u1ec1n Gradient m\u00e0n h\u00ecnh</label>";

    html += "</div>";  // end section 3

    // ═══════════════════════════════════════════════════════════════
    // Save + Action Buttons
    // ═══════════════════════════════════════════════════════════════
    html += R"rawhtml(
</form>
</div></body></html>
)rawhtml";

    server_.send(200, "text/html", html);
}

void WebConfigServer::handleSave() {
    // Target date/time
    if (server_.hasArg("datetime")) {
        String dt = server_.arg("datetime");
        int yr, mo, da, hr, mn;
        if (sscanf(dt.c_str(), "%d-%d-%dT%d:%d", &yr, &mo, &da, &hr, &mn) == 5) {
            struct tm tm_target = {0};
            tm_target.tm_year = yr - 1900;
            tm_target.tm_mon  = mo - 1;
            tm_target.tm_mday = da;
            tm_target.tm_hour = hr;
            tm_target.tm_min  = mn;
            tm_target.tm_sec  = 0;
            tm_target.tm_isdst = -1;
            time_t newEpoch = mktime(&tm_target);
            if (newEpoch != -1) {
                state_->setTargetEpoch(newEpoch);
            }
        }
    }

    // ── Common Settings ──────────────────────────────────────────
    state_->setLedEnabled(server_.hasArg("ledState") && server_.arg("ledState") == "1");

    state_->setSplashEnabled(server_.hasArg("splashState") && server_.arg("splashState") == "1");
    splash_->setEnabled(state_->isSplashEnabled());

    if (server_.hasArg("brightness")) {
        int val = server_.arg("brightness").toInt();
        state_->setBrightness(val);
        display_->setBrightness(state_->getBrightness());
    }

    if (server_.hasArg("screenMode")) {
        state_->setScreenMode(server_.arg("screenMode").toInt());
    }

    // ── Countdown Settings ───────────────────────────────────────
    if (server_.hasArg("customText")) {
        state_->setCustomText(server_.arg("customText"));
    }

    state_->setShowSeconds(server_.hasArg("showSeconds") && server_.arg("showSeconds") == "1");
    state_->setRainbowEnabled(server_.hasArg("rainbowState") && server_.arg("rainbowState") == "1");

    if (server_.hasArg("textSpeed")) {
        state_->setTextSpeed(server_.arg("textSpeed").toInt());
    }

    if (server_.hasArg("rainbowSpeed")) {
        state_->setRainbowSpeed(server_.arg("rainbowSpeed").toInt());
    }

    // ── Sensor Settings ──────────────────────────────────────────
    state_->setSensorBgEnabled(server_.hasArg("sensorBg") && server_.arg("sensorBg") == "1");

    // Redirect back to config page
    server_.sendHeader("Location", "/", true);
    server_.send(302, "text/plain", "Saved");
}

void WebConfigServer::handleRestart() {
    server_.send(200, "text/html",
        "<html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1'>"
        "</head><body style='background:#1a1a2e; color:#eee; text-align:center; font-family:Arial; padding-top:50px;'>"
        "<h2>&#128260; &#272;ang kh&#7903;i &#273;&#7897;ng l&#7841;i...</h2>"
        "<p>Vui l&#242;ng &#273;&#7907;i v&#224;i gi&#226;y v&#224; k&#7871;t n&#7889;i l&#7841;i WiFi n&#7871;u c&#7847;n.</p>"
        "</body></html>");
    delay(1000);
    ESP.restart();
}

void WebConfigServer::handleShowSplash() {
    server_.send(200, "text/html",
        "<html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1'>"
        "<meta http-equiv='refresh' content='11;url=/'></head>"
        "<body style='background:#1a1a2e; color:#eee; text-align:center; font-family:Arial; padding-top:50px;'>"
        "<h2>&#10024; &#272;ang Ph&#225;t Splash Screen...</h2>"
        "<p>T&#7921; &#273;&#7897;ng quay l&#7841;i sau 10 gi&#226;y.</p>"
        "</body></html>");
    splash_->play(*display_, nullptr, true);
}
