<script type="text/javascript">
<?php
    define("PATH", "../../../");
    include "../../../db/dbhelper.php";
    define("FLAG","flag");
    $dbhelper1 = new DAL();
    $sta_info = $dbhelper1->getall("select sta_mac,sta_x,sta_y from file_data where sta_mac='".$_GET["sta_mac"]."'");    
    if(count($sta_info)>0){
    $sta = $sta_info[0];
?>

    parent.sta.sta_x1.value=parent.sta.sta_x.value;
    parent.sta.sta_y1.value=parent.sta.sta_y.value;
    parent.sta.sta_x.value="<?php echo $sta["sta_x"];?>";
    parent.sta.sta_y.value="<?php echo $sta["sta_y"];?>";
    if(parent.sta.sta_mac.value == "" || parent.sta.sta_mac.value == null){
        parent.sta.sta_mac.value="<?php echo $sta["sta_mac"];?>";
    }
<?php
    }else{
?>  
    parent.sta.sta_x1.value="";
    parent.sta.sta_y1.value="";
    parent.sta.sta_x.value="";
    parent.sta.sta_y.value="";
    parent.sta.sta_mac.value = "";
<?php
    }
?>
</script>