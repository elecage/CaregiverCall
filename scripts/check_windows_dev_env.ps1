$ErrorActionPreference = "Continue"

Write-Host "CaregiverCall Windows development environment check"
Write-Host ""

Write-Host "[ESP-IDF / EIM]"
if (Get-Command idf.py -ErrorAction SilentlyContinue) {
    idf.py --version
} else {
    Write-Host "idf.py was not found in PATH. Open an ESP-IDF shell or install ESP-IDF with Espressif Installation Manager."
}

if (Get-Command eim.exe -ErrorAction SilentlyContinue) {
    eim.exe --version
} else {
    Write-Host "eim.exe was not found in PATH. Install Espressif.EIM-CLI if you want CLI-based ESP-IDF installation."
}

Write-Host ""
Write-Host "[WSL]"
if (Get-Command wsl.exe -ErrorAction SilentlyContinue) {
    wsl.exe --status
    Write-Host ""
    wsl.exe --list --verbose
} else {
    Write-Host "wsl.exe was not found. Install WSL2 first."
}

Write-Host ""
Write-Host "[USB/IP]"
if (Get-Command usbipd.exe -ErrorAction SilentlyContinue) {
    usbipd.exe list
} else {
    Write-Host "usbipd.exe was not found. Install usbipd-win if WSL2 needs direct USB flashing."
}

Write-Host ""
Write-Host "[Serial Ports]"
try {
    Get-CimInstance Win32_SerialPort -ErrorAction Stop |
        Select-Object DeviceID, Name, Description |
        Format-Table -AutoSize
} catch {
    Write-Host "Win32_SerialPort query failed. Falling back to .NET SerialPort API."
    try {
        [System.IO.Ports.SerialPort]::GetPortNames() | Sort-Object | ForEach-Object {
            Write-Host $_
        }
    } catch {
        Write-Host "Serial port query failed. Check Device Manager manually."
    }
}

Write-Host ""
Write-Host "Expected development serial port: COM10"
