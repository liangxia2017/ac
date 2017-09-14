<script type="text/javascript">
	while(parent.document.getElementById("sta_all").children.length>0){
		parent.document.getElementById("sta_all").removeChild(parent.document.getElementById("sta_all").children[0]);
	}
<?php
    if(file_exists("/tmp/sta_coordinate.s3db")){
    define("PATH", "../../../");
    include "../../../db/dbhelper.php";
    define("FLAG","flag");
    $dbhelper1 = new DAL();
    $sta_info = $dbhelper1->getall("select sta_mac,sta_x,sta_y from file_data where area_name='".$_GET["area_name"]."'");
    for($j = 0;$j<count($sta_info); $j++){
        $sta = $sta_info[$j];
        $sta_mac = $sta["sta_mac"];
        $mac = "";
        for($k = 0; $k<strlen($sta_mac); $k++){
            if($k != 0 && $k%2 == 0)
                $mac = $mac.":".$sta_mac[$k];
            else
                $mac = $mac.$sta_mac[$k];
        }
        echo "var div".$j." = parent.document.createElement('div');";
        echo "var img = parent.document.createElement('img');";
        echo "var div_mac".$j." = parent.document.createElement('div');";
        echo "var x".$j." = ".$sta["sta_x"]." +  ".$_GET["coord_x"]." + parent.document.body.clientLeft;";
        
        echo "var y".$j." = ".$_GET["coord_y"]." - parent.document.body.clientTop - ".$sta["sta_y"].";";
        echo "div".$j.".className = 'css';";            
        echo "div".$j.".style.left= x".$j." - 8 + 'px';";
        echo "div".$j.".style.top= y".$j."- 9 + 'px';";
        echo "div".$j.".style.display= 'block';";
        echo "div".$j.".style.zIndex= '".$j."*100 + 2100';";
        echo "img.src='".PATH."images/sta.png';";
        echo "img.width='16';";
        echo "img.height='18';";
        echo "img.title = '".$mac."';";
        echo "div_mac".$j.".innerHTML= '(".$sta["sta_x"].",".$sta["sta_y"].")';";
        echo "div_mac".$j.".style.left= x".$j."- 23 + 'px';";
        echo "div_mac".$j.".style.top= y".$j."- 28 + 'px';";      
        echo "div_mac".$j.".style.zIndex= '".$j."*100 + 2150';";
        echo "div_mac".$j.".style.position= 'absolute';";
        echo "div".$j.".appendChild(img);";
        echo "parent.document.getElementById('sta_all').appendChild(div_mac".$j.");";
        echo "parent.document.getElementById('sta_all').appendChild(div".$j.");";                
            }
        }
?>
</script>