<?php
/*
define("PATH", "../../../../");
include PATH."db/dbhelper.php";

//修改操作
if(isset($_POST["edit_id"]) && $_POST["edit_id"]!=""){
        $dbhelper = new DAL();
        $ap_group_name = $dbhelper->getall("select * from ap_group where id !=".$_POST["edit_id"]);
        foreach($ap_group_name as $name){
        		if($name["ap_group_name"] == $_POST["edit_ap_group_name"]){
        				echo "<script>alert('名称已存在');location='ap_group_config.php?r='+Math.random();</script>";
        			}
        	}
        $update = $dbhelper->update("update ap_group set ap_group_name='".trim($_POST["edit_ap_group_name"])."',max_ap='".$_POST["license"]."' where id=".$_POST["edit_id"]);
   	    if($update>0){
		  echo "<script>alert('修改成功');location='ap_group_config.php?r='+Math.random();</script>";
    	}
    }else if(isset($_POST["edit_ap_group_name"]) && $_POST["edit_ap_group_name"]!=""){
        $dbhelper = new DAL();
        $ap_group_name = $dbhelper->getall("select * from ap_group");
        foreach($ap_group_name as $name){
        		if($name["ap_group_name"] == $_POST["edit_ap_group_name"]){
        				echo "<script>alert('名称已存在');location='ap_group_config.php?r='+Math.random();</script>";
        			}
        	}
        $sql = "insert into ap_group(ap_group_name,max_ap) values('".trim($_POST["edit_ap_group_name"])."',".$_POST["license"].")";
        //echo $sql;
        $insert = $dbhelper->insert($sql);
   	    if($insert>0){
		  echo "<script>alert('添加成功');location='ap_group_config.php?r='+Math.random();</script>";
    	}
    }
*/
?>
<?php
define("PATH", "../../../../");
include PATH."db/dbhelper.php";

//修改操作
if(isset($_POST["edit_id"]) && $_POST["edit_id"]!=""){
        $dbhelper = new DAL();
        $ap_group_name = $dbhelper->getall("select * from ap_group where id !=".$_POST["edit_id"]);
        foreach($ap_group_name as $name){
    		if($name["ap_group_name"] == $_POST["edit_ap_group_name"]){
    				echo "<script>alert('名称已存在');location='ap_group_config.php?r='+Math.random();</script>";
    			}
    	}
        $sql = "select count(*) from ap_info where ap_group_name=(select ap_group_name from ap_group where id =".$_POST["edit_id"].")";
        $real_ap_num = $dbhelper->getOne($sql);
        if($real_ap_num > $_POST["license"]){
            echo "<script>alert('修改失败!所设置的license数小于当前AP数,请先删除部分AP!');location='ap_group_config.php?r='+Math.random();</script>";
        }else{
            $update = $dbhelper->update("update ap_group set ap_group_name='".trim($_POST["edit_ap_group_name"])."',max_ap='".$_POST["license"]."',sta_blance_sw='".$_POST["sta_blance_sw"]."' where id=".$_POST["edit_id"]);
       	    if($update>0){
    		  echo "<script>alert('修改成功');location='ap_group_config.php?r='+Math.random();</script>";
        	}
        }
    }else if(isset($_POST["edit_ap_group_name"]) && $_POST["edit_ap_group_name"]!=""){
        $dbhelper = new DAL();
        $ap_group_name = $dbhelper->getall("select * from ap_group");
        foreach($ap_group_name as $name){
        		if($name["ap_group_name"] == $_POST["edit_ap_group_name"]){
        				echo "<script>alert('名称已存在');location='ap_group_config.php?r='+Math.random();</script>";
        			}
        	}
            
        $sum = $dbhelper->getOne("select sum(max_ap) from ap_group where ap_group_name != 'unknown'");
        $license_path = "/ac/config/license.bin";
        if(file_exists($license_path)){
            exec("/ac/sbin/read_block -l /ac/config/license.bin",$arr,$inter);
            if($inter > 0 && $inter < 254){
                if($inter*16 - $sum > 0)
                    $left_license = $inter*16 - $sum;
                else
                    $left_license = 0;
            }else{
                if(16 - $sum > 0)
                    $left_license = 16 - $sum;
                else
                    $left_license = 0;
            }
        }else{
            if(16 - $sum > 0)
                    $left_license = 16 - $sum;
                else
                    $left_license = 0;
        }
        if($_POST["license"] > $left_license){
            echo "<script>alert('剩余license不足!');location='ap_group_config.php?r='+Math.random();</script>";
        }else{
        $sql = "insert into ap_group(ap_group_name,max_ap,sta_blance_sw) values('".trim($_POST["edit_ap_group_name"])."',".$_POST["license"].",".$_POST["sta_blance_sw"].")";
        $insert = $dbhelper->insert($sql);
   	    if($insert>0){
		  echo "<script>alert('添加成功');location='ap_group_config.php?r='+Math.random();</script>";
    	}
    }

    }

?>