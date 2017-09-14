<?php
define("PATH", "../../../../");
include PATH."db/dbhelper.php";

if(isset($_FILES["import"])){
    $file = $_FILES["import"];
    $path = $file["tmp_name"];
    $data = fopen($path,'r');
    $row = fgetcsv($data);//表头
    $sql = "insert into ap_info(";
    for($i = 0; $i<count($row); $i++){
        if($i == count($row)-1){
            $sql = $sql.$row[$i].") values(";
        }else{
            $sql = $sql.$row[$i].",";
        }
    }
    for($i = 0; $i<count($row); $i++){
        if($i == count($row)-1){
            $sql = $sql."?)";
        }else{
            $sql = $sql."?,";
        }
    }
    $dbhelper = new DAL();
    $dbhelper->delete("delete from ap_info");
    while($row = fgetcsv($data)){        
        $dbhelper->insert($sql,$row);
    }
    echo "<script>alert('导入成功!');location='ap_group_config.php?r='+Math.random();</script>";
}
?>