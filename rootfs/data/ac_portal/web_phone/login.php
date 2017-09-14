<?php
    include "/ac/htmls/db/dbhelper.php";
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
        //$cmd = "/ac/script/ipset_do 1 ".$ip;
       // exec($cmd,$a,$in);
        $cmd = 'ip neigh |grep '.$_SERVER["REMOTE_ADDR"].' -w |cut -d " " -f 5 | tr -d ":"';
				exec($cmd,$arr,$inter);
				$mac = implode($arr);
				$dbhelper = new DAL();
				$ac_ip = $dbhelper->getOne("select redirect_ip from ac_basic_conf");
				//echo "<script>location='http://".$ac_ip."/index.html';</script>";
				Header("HTTP/1.1 302 Moved Permanently");
				Header("Location:http://".$ac_ip."/success.php?sta_ip=".$_SERVER["REMOTE_ADDR"]."&mac=".$mac);
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