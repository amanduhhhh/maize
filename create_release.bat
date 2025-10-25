@echo off
echo Creating Oubliette Release Package...

REM Create release directory
if exist "release" rmdir /s /q "release"
mkdir "release"

REM Copy the executable
copy "build\Release\Oubliette.exe" "release\"

REM Copy SFML DLLs
copy "SFML-2.6.1\bin\sfml-graphics-2.dll" "release\"
copy "SFML-2.6.1\bin\sfml-window-2.dll" "release\"
copy "SFML-2.6.1\bin\sfml-system-2.dll" "release\"
copy "SFML-2.6.1\bin\openal32.dll" "release\"

REM Copy game instructions
echo # Oubliette - Maze Chase Game > "release\README.txt"
echo. >> "release\README.txt"
echo A maze-based chase game where you must collect a key before escaping! >> "release\README.txt"
echo. >> "release\README.txt"
echo HOW TO PLAY: >> "release\README.txt"
echo - You are the blue square >> "release\README.txt"
echo - Find the yellow key and collect it >> "release\README.txt"
echo - Escape through the brown exit doors >> "release\README.txt"
echo - Avoid the red enemies that chase you >> "release\README.txt"
echo - Each round has more enemies and a new maze >> "release\README.txt"
echo. >> "release\README.txt"
echo CONTROLS: >> "release\README.txt"
echo - Arrow Keys: Move your character >> "release\README.txt"
echo. >> "release\README.txt"
echo TO START: >> "release\README.txt"
echo - Double-click Oubliette.exe >> "release\README.txt"
echo. >> "release\README.txt"
echo Good luck escaping the maze! >> "release\README.txt"

echo.
echo Creating ZIP archive...

REM Create ZIP file
powershell Compress-Archive -Path "release\*" -DestinationPath "Oubliette-v1.0.zip" -Force

echo.
echo Release package created!
echo - Folder: release\
echo - Archive: Oubliette-v1.0.zip
echo.
echo Contents:
dir "release" /b
echo.
echo Upload 'Oubliette-v1.0.zip' to GitHub Releases!
pause
