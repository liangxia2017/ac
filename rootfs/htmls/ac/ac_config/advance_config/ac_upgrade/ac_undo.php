<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<?php
if($_GET["action"] == "backup"){
    $cmd = "/ac/script/system_backup";
    exec($cmd,$arr,$int);
    if($int == 255){
        echo "<script>alert('还原点创建失败！还原点数量超过5个！');location='ac_upgrade.php';</script>";
    }else if($int != 0){
        echo "<script>alert('还原点创建失败！');location='ac_upgrade.php';</script>";
    }else{
        echo "<script>alert('还原点创建成功！');location='ac_upgrade.php';</script>";
    }
}
    $new_runtime = $_GET["undo"];
if($_GET["action"] == "restore"){
    $cmd = "echo 'RESTORE_NAME=".$new_runtime."' > /opt/micro_ac/tmp/system_upg.conf";
    exec($cmd,$arr,$int);
    if($int != 0){
        echo "<script>alert('还原失败！');location='ac_upgrade.php';</script>";
    }
}
if($_GET["action"] == "delete"){
    $cmd = "rm -rf /opt/micro_ac/".$new_runtime;
    exec($cmd,$arr,$int);
    if($int != 0){
        echo "<script>alert('删除失败！');location='ac_upgrade.php';</script>";
    }else{
        echo "<script>alert('删除成功！');location='ac_upgrade.php';</script>";
    }
}
?>
<body>
<div style='text-align: center; margin-top: 10%'><label style="color: red;" id="upgrading"></label></div>
</body>
<script>
var num = 90;    
function upgrading(){
    num--;
    if(num == 0){
    		alert('还原成功！');
        top.location = '../../../../logout.php';
    }
    var upg = document.getElementById("upgrading");
    var msg = "系统还原中...(剩余"+num+"秒)";
    upg.innerHTML = msg;
}
upgrading();
setInterval('upgrading()',1000);
</script>
</html>