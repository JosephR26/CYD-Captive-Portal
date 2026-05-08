#pragma once

const char login_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body { font-family: 'Courier New', Courier, monospace; background-color: #000; color: #0ff; text-align: center; padding: 50px; }
        .container { border: 2px solid #0ff; padding: 20px; display: inline-block; box-shadow: 0 0 15px #0ff; }
        h1 { text-transform: uppercase; letter-spacing: 5px; }
        input { background: #000; border: 1px solid #f0f; color: #f0f; padding: 10px; margin: 10px; width: 80%; }
        input[type="submit"] { background: #f0f; color: #000; font-weight: bold; cursor: pointer; }
        input[type="submit"]:hover { background: #0ff; color: #000; border-color: #0ff; }
        .glitch { animation: glitch 1s linear infinite; }
        @keyframes glitch {
            2%, 64% { transform: translate(2px,0) skew(0deg); }
            4%, 60% { transform: translate(-2px,0) skew(0deg); }
            62% { transform: translate(0,0) skew(5deg); }
        }
    </style>
</head>
<body>
    <div class="container">
        <h1 class="glitch">System Login</h1>
        <p>Authentication Required to Access Network</p>
        <form action="/login" method="POST">
            <input type="text" name="user" placeholder="USERNAME" required><br>
            <input type="password" name="pass" placeholder="PASSWORD" required><br>
            <input type="submit" value="ACCESS_GRANTED">
        </form>
    </div>
</body>
</html>
)rawliteral";

const char success_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body { font-family: 'Courier New', Courier, monospace; background-color: #000; color: #0ff; text-align: center; padding: 50px; }
        .container { border: 2px solid #0ff; padding: 20px; display: inline-block; box-shadow: 0 0 15px #0ff; }
        h1 { color: #f0f; text-transform: uppercase; }
    </style>
</head>
<body>
    <div class="container">
        <h1>ACCESS_GRANTED</h1>
        <p>Neural Link Established. You are now connected to the grid.</p>
    </div>
</body>
</html>
)rawliteral";
