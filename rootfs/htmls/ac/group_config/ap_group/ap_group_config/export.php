<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
<?php
//    $ap_group_name = $_GET["ap_group_name"];
    $name = "ap_info.csv";
    $path = ".output ".$name;
    $cmd = "/ac/sqlite/bin/sqlite3 -header -csv -cmd \"".$path."\" /ac/db/ac.s3db \"select ap_mac,ap_group_name,ap_remark,bg_channel,a_channel from ap_info order by ap_group_name\"";
    exec($cmd,$arr,$interval);
    if($interval != 0){
        echo "<script>alert('导出失败');location='ap_group_config.php?r='+Math.random();</script>";
    }
?>
<body>
<a id="export" href="#" onclick="this.href='<?php echo $name;?>'"></a>
</body>
<script type="text/javascript">
var e = document.getElementById("export");
e.click();
var k = 0;
var s;
function locat(){
    location = "ap_group_config.php?file=<?php echo $name;?>";    
}
setTimeout('locat()',1000);
</script>
</html>