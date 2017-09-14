<?php
session_start();
if(isset($_SESSION["code"])){
    if($_SESSION["code"] == $_GET["code"]){        
        unset($_SESSION["code"]);
        echo "web_phone/login.php?action=sms";
    }else{
        echo "fail";
    }
}else{
    echo "fail";
}
?>