<?php
    include "../../../htmls/db/dbhelper.php";
    define("FLAG","flag");
    $dbhelper1 = new DAL();
    $sta_info = $dbhelper1->getRow("select sta_mac,sta_x,sta_y from file_data where sta_mac='".$_GET["sta_mac"]."'");  
    $sta = (array)$sta_info;
?>
<script type="text/javascript">
    parent.sta.sta_x.value = "<?php echo $sta["sta_x"];?>";
    parent.sta.sta_y.value = "<?php echo $sta["sta_y"];?>";
    parent.sta.sta_mac.value = "<?php echo $sta["sta_mac"];?>";
</script>