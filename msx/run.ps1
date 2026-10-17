# Build and run the prototype in openMSX; usage: run.ps1 [seconds] -> screenshot in out/shot.png
param([int]$Seconds = 40, [string]$Machine = "NMS8250_256K", [string]$Ext = "msxdos2", [string]$Dir = "dos2")
$here = Split-Path -Parent $MyInvocation.MyCommand.Path
$env:MSXGL_PATH = Join-Path $here "..\MSXgl"
Set-Location $here
$node = Join-Path $env:MSXGL_PATH "tools\build\Node\node.exe"
& $node (Join-Path $env:MSXGL_PATH "engine\script\js\build.js") 2>&1 | Tee-Object -FilePath "$here\out\build.log" | Select-String -Pattern "error|warning|Size=|Higher=" 
$shot = $here.Replace("\", "/") + "/out/shot.png"
"after time $Seconds { screenshot {$shot} }`nafter time $($Seconds+2) { exit }" | Set-Content "$here\out\shot.tcl" -Encoding ascii
$p = Start-Process openmsx -ArgumentList "-machine",$Machine,"-ext",$Ext,"-diska","$here\emul\$Dir","-script","$here\out\shot.tcl" -PassThru
if (-not $p.WaitForExit(($Seconds + 60) * 1000)) { $p.Kill() }
