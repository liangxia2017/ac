<?php
session_start();
if(isset($_SESSION["code"])){
    if($_SESSION["code"] == $_GET["code"]){        
        unset($_SESSION["code"]);
     Header("Content-type:application/json");
			echo json_encode(array('result'=>0)); 
    }else{
     Header("Content-type:application/json");
			echo json_encode(array('result'=>1)); 
    }
}else{
     Header("Content-type:application/json");
			echo json_encode(array('result'=>1)); 
}
?>