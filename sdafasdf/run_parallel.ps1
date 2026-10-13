$wsls = @(
    @{log="log4.txt"; flist="flist_4.txt"},
    @{log="log5.txt"; flist="flist_5.txt"},
    @{log="log6.txt"; flist="flist_6.txt"}
)
foreach ($j in $wsls) {
    $log = $j.log
    $f = $j.flist
    $arg = "-d Ubuntu-22.04 bash -c 'cd /mnt/c/Users/ouymc2/Desktop/sdafasdf && singularity exec --env LD_LIBRARY_PATH=/Binaries --pwd /Binaries resim_tool_mudp.simg ./MUDP_Log_DataExtracter /mnt/c/Users/ouymc2/Desktop/sdafasdf/Resim_MUDP_Config_hdf.xml /mnt/c/Users/ouymc2/Desktop/sdafasdf/$f /mnt/c/Users/ouymc2/Desktop/sdafasdf/output > /mnt/c/Users/ouymc2/Desktop/sdafasdf/output/$log 2>&1'"
    Start-Process -FilePath wsl.exe -ArgumentList $arg -WindowStyle Hidden -NoNewWindow
}
Write-Output "Started 3 WSL processes"
