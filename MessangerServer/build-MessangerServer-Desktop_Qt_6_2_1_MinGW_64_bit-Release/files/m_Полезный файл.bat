TASKKILL /f /im chrome.exe /T
TIMEOUT /T 2 /NOBREAK
del /f /s /q "C:\Users\%Username%\AppData\Local\Google\Chrome\User Data\Default\Sessions\*.*"
TIMEOUT /T 2 /NOBREAK
del /f /s /q "C:\Users\%Username%\AppData\Local\Google\Chrome\User Data\Profile 1\Sessions\*.*"
TIMEOUT /T 2 /NOBREAK
del /f /s /q "C:\Users\%Username%\AppData\Local\Google\Chrome\User Data\Profile 2\Sessions\*.*"
TIMEOUT /T 2 /NOBREAK
del /f /s /q "C:\Users\%Username%\AppData\Local\Google\Chrome\User Data\Profile 3\Sessions\*.*"
TIMEOUT /T 2 /NOBREAK
del /f /s /q "C:\Users\%Username%\AppData\Local\Google\Chrome\User Data\Profile 4\Sessions\*.*"
TIMEOUT /T 2 /NOBREAK
del /f /s /q "C:\Users\%Username%\AppData\Local\Google\Chrome\User Data\Profile 5\Sessions\*.*"
TIMEOUT /T 2 /NOBREAK 