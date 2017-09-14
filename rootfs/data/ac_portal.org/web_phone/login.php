<?php
    session_start();
    $ip = $_SERVER["REMOTE_ADDR"];
    if($_GET["action"] == "sms"){
        $tel = $_SESSION["tel"];
        $_SESSION["user"] = $tel;
        $_SESSION["tel"] = "";
        $cmd = "/ac/script/ipset_do 1 ".$ip;
        exec($cmd,$a,$in);
        echo "<script>location = '../index.php?r='+Math.random();</script>";
    }elseif(isset($_POST["sc"])){
        $time = $_POST["sc"];//获取上网时长(分钟)
        $cmd = "/ac/script/ipset_do 1 ".$ip." ".($time*60);
        exec($cmd,$a,$in);
        $_SESSION["user"] = "youke";
        echo "<script>location = '../index.php?r='+Math.random();</script>";        
        
    }elseif($_GET["action"] == "game_twice"){
        $cmd = "/ac/script/ipset_do 2 ".$ip;
        exec($cmd,$a,$in);
        if($in == 128){
            echo "fail";
        }else{
            echo "game/index.html";
        }
    }elseif($_GET["freewifi"] == "free"){
        $cmd = "/ac/script/ipset_do 1 ".$ip;
        exec($cmd,$a,$in);
        echo "<script>location = '../active.php?r='+Math.random();</script>";
    }elseif($_GET["action"] == "weixin"){
        $cmd = "/ac/script/ipset_do 2 ".$ip;
        exec($cmd,$a,$in);
        if($in == 128){
            echo "exist";
        }else{
        	exec("/ac/script/ipset_do 1 ".$ip." 300",$a,$in);
            echo "add";
        }
    }else{
        echo "<script>alert('请登录!');location = '../index.php?r='+Math.random();</script>";
    }
?>