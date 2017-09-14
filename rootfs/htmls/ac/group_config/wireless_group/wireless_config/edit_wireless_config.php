<?php
define("PATH", "../../../../");
include PATH."db/dbhelper.php";

if(isset($_POST["id"]) && $_POST["id"]!=""){
    $dbhelper = new DAL();
    if($_POST["radio_card_id"] === "1"){
        $params = array($_POST["txpower"],$_POST["auto_power_time"],$_POST["bg_wireless_mode"],$_POST["data_stream"],$_POST["channel_width"],$_POST['bg_beacon_rate_set'],
        $_POST["short_gi"],$_POST["ampdu"],$_POST["amsdu"],$_POST["beacon_intval"],$_POST["rts"],$_POST["auto_channel_sw"],$_POST["auto_channel_mode"],$_POST["auto_channel_period"],
        $_POST["first_5G"],$_POST["weak_rssi"],$_POST["radar_sw"],$_POST["id"]);	
        $sql = "update wireless_config set bg_txpower=?, bg_auto_power_time=?, bg_wireless_mode=?, bg_data_stream=?,bg_channel_width=?,bg_beacon_rate_set=?,
        bg_short_gi=?,bg_ampdu=?,bg_amsdu=?,beacon_intval=?,rts=?,auto_channel_sw=?,auto_channel_mode=?,auto_channel_period=?,first_5G=?,weak_rssi_refuse=?,close_radar=? where id=?";
    }else{
        $params = array($_POST["txpower"],$_POST["auto_power_time"],$_POST["a_wireless_mode"],$_POST["data_stream"],$_POST["channel_width"],$_POST["a_beacon_rate_set"],
        $_POST["short_gi"],$_POST["ampdu"],$_POST["amsdu"],$_POST["beacon_intval"],$_POST["rts"],$_POST["auto_channel_sw"],$_POST["auto_channel_mode"],$_POST["auto_channel_period"],
        $_POST["first_5G"],$_POST["weak_rssi"],$_POST["radar_sw"],$_POST["id"]);	
        $sql = "update wireless_config set a_txpower=?, a_auto_power_time=?, a_wireless_mode=?, a_data_stream=?,a_channel_width=?,a_beacon_rate_set=?,
        a_short_gi=?,a_ampdu=?,a_amsdu=?,beacon_intval=?,rts=?,auto_channel_sw=?,auto_channel_mode=?,auto_channel_period=?,first_5G=?,weak_rssi_refuse=?,close_radar=? where id=?";
    }
    $update = $dbhelper->update($sql,$params);
    if($update>0){
        echo "<script>alert('修改成功!');location='wireless_config.php?group_name=".$_GET["group_name"]."';</script>";
   	    }
}else{
    $dbhelper = new DAL();
    if($_POST["radio_card_id"] === "1"){
        $params = array($_GET["group_name"],$_POST["txpower"],$_POST["auto_power_time"],$_POST["bg_wireless_mode"],$_POST["data_stream"],$_POST["channel_width"],$_POST["bg_beacon_rate_set"],
        $_POST["short_gi"],$_POST["ampdu"],$_POST["amsdu"],$_POST["beacon_intval"],$_POST["rts"],$_POST["auto_channel_sw"],$_POST["auto_channel_mode"],$_POST["auto_channel_period"],
        $_POST["first_5G"],$_POST["weak_rssi"],$_POST["radar_sw"]);	
        $sql = "insert into wireless_config(wireless_group_name, bg_txpower, bg_auto_power_time, bg_wireless_mode, bg_data_stream,bg_channel_width,bg_beacon_rate_set,
        bg_short_gi,bg_ampdu,bg_amsdu,beacon_intval,rts,auto_channel_sw,auto_channel_mode,auto_channel_period,first_5G,weak_rssi_refuse,close_radar) values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";
    }else{
        $params = array($_GET["group_name"],$_POST["txpower"],$_POST["auto_power_time"],$_POST["a_wireless_mode"],$_POST["data_stream"],$_POST["channel_width"],$_POST["a_beacon_rate_set"],
        $_POST["short_gi"],$_POST["ampdu"],$_POST["amsdu"],$_POST["beacon_intval"],$_POST["rts"],$_POST["auto_channel_sw"],$_POST["auto_channel_mode"],$_POST["auto_channel_period"],
        $_POST["first_5G"],$_POST["weak_rssi"],$_POST["radar_sw"]);	
        $sql = "insert into wireless_config(wireless_group_name, a_txpower, a_auto_power_time, a_wireless_mode, a_data_stream, a_channel_width, a_beacon_rate_set,
        a_short_gi,a_ampdu,a_amsdu,beacon_intval,rts,auto_channel_sw,auto_channel_mode,auto_channel_period,first_5G,weak_rssi_refuse,close_radar) values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";
    }
    $update = $dbhelper->update($sql,$params);
    if($update>0){
        echo "<script>alert('配置成功!');location='wireless_config.php?group_name=".$_GET["group_name"]."';</script>";
   	    }
    }
?>