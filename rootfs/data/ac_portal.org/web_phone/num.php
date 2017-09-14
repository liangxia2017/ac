<?php
//session_start();
$filePath = "count.txt";
$file = fopen($filePath,"r");
//var_dump($count);
//if($_SESSION["count"] == ""){
//    $_SESSION["count"] = $count;
//}
//$count = $_SESSION["count"] + 1;
$count = fread($file,filesize($filePath));
fclose($file);
$count ++;
$file = fopen($filePath,"w");
fwrite($file,$count);
fclose($file);
?>
<html>
num:<?php echo $count;?>
</html>