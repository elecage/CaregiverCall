$ErrorActionPreference = "Stop"

$RepoRoot = Resolve-Path (Join-Path $PSScriptRoot "..")
$VenvPath = Join-Path $RepoRoot ".venv"
$RequirementsPath = Join-Path $RepoRoot "requirements.txt"
$PythonExe = Join-Path $VenvPath "Scripts\python.exe"

if (Get-Command python -ErrorAction SilentlyContinue) {
    $PythonCommand = @("python")
} elseif (Get-Command py -ErrorAction SilentlyContinue) {
    $PythonCommand = @("py", "-3")
} else {
    throw "Python was not found. Install Python 3 and make sure 'python' or 'py' is available in PATH."
}
$PythonArgs = @()
if ($PythonCommand.Length -gt 1) {
    $PythonArgs = $PythonCommand[1..($PythonCommand.Length - 1)]
}

if (-not (Test-Path $VenvPath)) {
    & $PythonCommand[0] @PythonArgs -m venv $VenvPath
    if ($LASTEXITCODE -ne 0) {
        throw "Failed to create the virtual environment. Install Python 3 with the venv module enabled."
    }
}

if (-not (Test-Path $PythonExe)) {
    throw "Virtual environment was not created correctly. Expected Python at $PythonExe"
}

& $PythonExe -m pip install --upgrade pip

if (Test-Path $RequirementsPath) {
    & $PythonExe -m pip install -r $RequirementsPath
}

Write-Host "Python virtual environment is ready at $VenvPath"
Write-Host "Activate it with: .\.venv\Scripts\Activate.ps1"
