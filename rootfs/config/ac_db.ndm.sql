CREATE TABLE "ac_dhcp_server" (
"id" INTEGER NOT NULL,
"start_ip" TEXT,
"end_ip" TEXT,
"mask" TEXT,
"gateway" TEXT,
"dns" TEXT,
"lease" INTEGER,
"option43" TEXT,
"option60" TEXT,
"net_id" INTEGER,
"option82" TEXT,
PRIMARY KEY ("id") 
);

CREATE TABLE "ac_network_config" (
"id" INTEGER NOT NULL,
"type" INTEGER,
"network_card" TEXT,
"get_ip_method" INTEGER,
"ip" TEXT,
"mask" TEXT,
"net_id" INTEGER,
"pppoe_user" TEXT,
"pppoe_psw" TEXT,
PRIMARY KEY ("id") 
);

CREATE TABLE "ac_basic_conf" (
"id" INTEGER NOT NULL,
"ntp_server_ip" TEXT,
"central_forward_mode" INTEGER,
"tunnel_out" TEXT,
"tunnel_in_ip" TEXT,
"ac_portal_sw" INTEGER,
"redirect_ip" TEXT,
"dns1" TEXT,
"dns2" TEXT,
"location_switch" INTEGER,

"portal_white_list" TEXT,

"time_reboot_sw" INTEGER DEFAULT 0,

"timer" INTEGER DEFAULT 3,

PRIMARY KEY ("id") 
);

CREATE TABLE "ac_user_info" (
"id" INTEGER NOT NULL,
"user_name" TEXT,
"password" TEXT,
"user_type" TEXT,
PRIMARY KEY ("id") ,
CONSTRAINT "user_name" UNIQUE ("user_name")
);

CREATE TABLE "group_relation" (
"id" INTEGER NOT NULL,
"ap_group_name" TEXT,
"wlan_group_name" TEXT,
"wireless_group_name" TEXT,
"function_group_name" TEXT,
PRIMARY KEY ("id") ,
CONSTRAINT "fkey0" FOREIGN KEY ("wlan_group_name") REFERENCES "wlan_group" ("wlan_group_name"),
CONSTRAINT "fk_group_relastion_group_relastion_1" FOREIGN KEY ("ap_group_name") REFERENCES "ap_group" ("ap_group_name"),
CONSTRAINT "fk_group_relation_group_relation_1" FOREIGN KEY ("wireless_group_name") REFERENCES "wireless_group" ("wireless_group_name"),
CONSTRAINT "fk_group_relation_group_relation_2" FOREIGN KEY ("function_group_name") REFERENCES "function_group" ("function_group_name")
);

CREATE TABLE "ap_info" (
"id" INTEGER NOT NULL,
"ap_mac" TEXT NOT NULL,
"ap_group_name" TEXT,
"ap_ip" TEXT,
"ap_remark" TEXT,
"status" INTEGER DEFAULT 0,
"last_join_time" TEXT,
"bg_channel" INTEGER,
"a_channel" INTEGER,
"ap_locate_area" TEXT,
"ap_x" INTEGER,
"ap_y" INTEGER,
"config_mask" INTEGER,
"config_status" BLOB,
"refer_rssi" INTEGER DEFAULT 57,
"soft_ver" TEXT,

"sta_num" INTEGER DEFAULT 0,

PRIMARY KEY ("id") ,
CONSTRAINT "fk_ap_info_ap_info_1" FOREIGN KEY ("ap_locate_area") REFERENCES "ap_locate_edit" ("area_name"),
CONSTRAINT "fk_ap_info_ap_info_2" FOREIGN KEY ("ap_group_name") REFERENCES "ap_group" ("ap_group_name"),
CONSTRAINT "ap_mac" UNIQUE ("ap_mac")
);

CREATE TABLE "ap_upgrade_config" (
"id" INTEGER NOT NULL,
"img_version" TEXT,
"img_name" TEXT,
PRIMARY KEY ("id") ,
CONSTRAINT "img_version" UNIQUE ("img_version")
);

CREATE TABLE "ap_version" (
"id" INTEGER NOT NULL,
"manufacturer" TEXT,
"hardware_version" TEXT,
"product_model" TEXT,
PRIMARY KEY ("id") 
);

CREATE TABLE "func_config" (
"id" INTEGER NOT NULL,
"function_group_name" TEXT,
"link_check_sw" INTEGER DEFAULT 0,

"link_check_action" INTEGER,
"keeplive_period" INTEGER DEFAULT 3,

"ap_ntp_sw" INTEGER DEFAULT 0,

"ap_ntp_period" INTEGER,
"ap_ntp_server" TEXT,
"ap_locate_sw" INTEGER DEFAULT 0,

"ap_locate_report_period" INTEGER,
"snmp_period" INTEGER DEFAULT 3,

"ap_url_sw" INTEGER DEFAULT 0,

"ap_url_str" TEXT,

"ap_white_list" TEXT,

"ap_log_sw" INTEGER DEFAULT 0,

"ap_log_period" INTEGER,

"ap_cmd_sw" INTEGER DEFAULT 0,

"ap_cmd" TEXT,

"dns_deny_sw" INTEGER DEFAULT 0,

"dns_deny" TEXT,

"rsync_sw" INTEGER DEFAULT 0,

"rsync_period" INTEGER DEFAULT 60,

"rsync_port" INTEGER DEFAULT 5241,

"rsync_ip" TEXT,

"dns_white" TEXT,

"event_sta_updown" INTEGER DEFAULT 0,

PRIMARY KEY ("id") ,
CONSTRAINT "fk_func_config_func_config_1" FOREIGN KEY ("function_group_name") REFERENCES "function_group" ("function_group_name")
);

CREATE TABLE "wireless_config" (
"id" INTEGER NOT NULL,
"wireless_group_name" TEXT,
"bg_txpower" INTEGER DEFAULT 6,
"bg_auto_power_time" INTEGER DEFAULT 0,
"bg_channel_width" INTEGER DEFAULT 0,
"bg_wireless_mode" INTEGER DEFAULT 4,
"bg_short_gi" INTEGER DEFAULT 1,
"bg_ampdu" INTEGER DEFAULT 1,
"bg_amsdu" INTEGER DEFAULT 1,
"bg_data_stream" INTEGER DEFAULT 3,
"bg_beacon_rate_set" TEXT,
"a_txpower" INTEGER DEFAULT 6,
"a_auto_power_time" INTEGER DEFAULT 0,
"a_channel_width" INTEGER DEFAULT 0,
"a_wireless_mode" INTEGER DEFAULT 7,
"a_short_gi" INTEGER DEFAULT 1,
"a_ampdu" INTEGER DEFAULT 1,
"a_amsdu" INTEGER DEFAULT 1,
"a_data_stream" INTEGER DEFAULT 3,
"a_beacon_rate_set" TEXT,
"beacon_intval" INTEGER DEFAULT 150,
"rts" INTEGER DEFAULT 2346,
"auto_channel_sw" INTEGER DEFAULT 0,
"auto_channel_mode" INTEGER DEFAULT 0,
"auto_channel_period" INTEGER DEFAULT 1200,
"first_5G" INTEGER DEFAULT 0,

"weak_rssi_refuse" INTEGER DEFAULT 0,

"close_radar" INTEGER DEFAULT 1,

PRIMARY KEY ("id") ,
CONSTRAINT "fk_wireless_config_wireless_config_1" FOREIGN KEY ("wireless_group_name") REFERENCES "wireless_group" ("wireless_group_name")
);

CREATE TABLE "wlan_config" (
"id" INTEGER NOT NULL,
"wlan_group_name" TEXT,
"wlan_id" INTEGER,
"ssid_hide_sw" INTEGER,
"max_user" INTEGER,
"vlan_id" INTEGER,
"ssid_up_traffic" INTEGER,
"ssid_down_traffic" INTEGER,
"user_up_traffic" INTEGER,
"user_down_traffic" INTEGER,
"ssid" TEXT,
"security_policy" TEXT,
"forward_mode" INTEGER,
PRIMARY KEY ("id") ,
CONSTRAINT "fk_wlan_config_wlan_security_policy_1" FOREIGN KEY ("security_policy") REFERENCES "wlan_security_policy" ("security_policy_name"),
CONSTRAINT "fk_wlan_config_wlan_group_1" FOREIGN KEY ("wlan_group_name") REFERENCES "wlan_group" ("wlan_group_name")
);

CREATE TABLE "wlan_group" (
"id" INTEGER NOT NULL,
"wlan_group_name" TEXT,
PRIMARY KEY ("id") ,
CONSTRAINT "wlan_group_name" UNIQUE ("wlan_group_name")
);

CREATE TABLE "wlan_security_policy" (
"id" INTEGER NOT NULL,
"security_policy_name" TEXT,
"auth_mode" INTEGER,
"encryption_mode" INTEGER,
"psk_key" TEXT,
"radius_auth_server" TEXT,
"radius_auth_port" INTEGER,
"radius_account_server" TEXT,
"radius_account_port" INTEGER,
"radius_key" TEXT,
PRIMARY KEY ("id") ,
CONSTRAINT "security_policy_name" UNIQUE ("security_policy_name")
);

CREATE TABLE "ap_locate_edit" (
"id" INTEGER NOT NULL,
"area_name" TEXT NOT NULL,
"map_path" TEXT,
"map_x" INTEGER,
"map_y" INTEGER,
PRIMARY KEY ("id") ,
CONSTRAINT "locate_area" UNIQUE ("area_name")
);

CREATE TABLE "ap_group" (
"id" INTEGER NOT NULL,
"ap_group_name" TEXT,
"max_ap" INTEGER,
"sta_blance_sw" INTEGER DEFAULT 0,

PRIMARY KEY ("id") ,
CONSTRAINT "ap_group_name" UNIQUE ("ap_group_name")
);

CREATE TABLE "wireless_group" (
"id" INTEGER NOT NULL,
"wireless_group_name" TEXT,
PRIMARY KEY ("id") ,
CONSTRAINT "wireless_group_name" UNIQUE ("wireless_group_name")
);

CREATE TABLE "function_group" (
"id" INTEGER NOT NULL,
"function_group_name" TEXT,
PRIMARY KEY ("id") ,
CONSTRAINT "function_group_name" UNIQUE ("function_group_name")
);

CREATE TABLE "sta_blacklist" (
"id" INTEGER,
"sta_mac" TEXT,
"ap_group_name" TEXT,
PRIMARY KEY ("id") ,
FOREIGN KEY ("ap_group_name") REFERENCES "ap_group" ("ap_group_name"),
CONSTRAINT "sta_mac" UNIQUE ("sta_mac")
);

CREATE TABLE "ap_snmp_info" (
"id" INTEGER NOT NULL,
"SysIPAddress" TEXT,
"SysIPNetMask" TEXT,
"SysGWAddr" TEXT,
"SysMacAddress" TEXT NOT NULL,
"SysUpTime" TEXT,
"SysOnlineTime" TEXT,
"SysModel" TEXT,
"SysManufacture" TEXT,
"SoftwareVersion" TEXT,
"CPURTUsage
" TEXT,
"MemRTUsage" TEXT,
"TxDataPkts" TEXT,
"RxDataPkts" TEXT,
"UplinkDataOctets" TEXT,
"DwlinkDataOctets
" TEXT,
"APACAssociateStatus" TEXT,
"ifOperStatus" TEXT,
"ifLastChange" TEXT,
"ApStationAssocSum
" TEXT,
"AssocTimes" TEXT,
"AssocFailTimes" TEXT,
"ApStatsDisassociated" TEXT,
"ifInUcastPkts" TEXT,
"ifInOctets" TEXT,
"ifOutUcastPkts
ifOutUcastPkts" TEXT,
"ifOutOctets" TEXT,
PRIMARY KEY ("id") 

);

CREATE TABLE "sta_list_assc" (
"id" INTEGER,
"sta_mac" TEXT,
"assc_ap_mac" TEXT,
"assc_time" TEXT,
"radio" INTEGER DEFAULT 1,

PRIMARY KEY ("id") ,
CONSTRAINT "fk_associationstainfo_ap_info_1" FOREIGN KEY ("assc_ap_mac") REFERENCES "ap_info" ("ap_mac"),
CONSTRAINT "fk_sta_list_assc_sta_list_assc_1" FOREIGN KEY ("sta_mac") REFERENCES "sta_snmp_info" ("sta_mac"),
CONSTRAINT "sta_mac" UNIQUE ("sta_mac")
);

CREATE TABLE "sta_snmp_info" (
"id" INTEGER,
"sta_mac" TEXT,
"assc_ap_mac" TEXT,
"sta_up_time" TEXT,
"StaIPAddress" TEXT,
"APReceivedStaSignalStrength" TEXT,
"APReceivedStaSNR" TEXT,
"StaTxPkts" TEXT,
"StaTxBytes" TEXT,
"StaRxPkts" TEXT,
"StaRxBytes" TEXT,
"StaRadioMode" TEXT,
"StaRadioChannel" TEXT,
"APTxRates" TEXT,
"StaVlanId" TEXT,
"StaSSIDName" TEXT,
PRIMARY KEY ("id") 
);

CREATE TABLE "route" (
"id" INTEGER,
"source_ip" TEXT,
"destination_ip" TEXT,
PRIMARY KEY ("id") 
);

CREATE TABLE "nat" (
"id" INTEGER,
"source_ip" TEXT,
"source_mask" TEXT,
"output_eth" TEXT,
PRIMARY KEY ("id") 
);

CREATE TABLE "ap_report" (

"id" INTEGER NOT NULL,

"ap_mac" TEXT NOT NULL,

"eth_re" INTEGER,

"eth_se" INTEGER,

"wifi_re" INTEGER,

"wifi_se" INTEGER,

"lte_re" INTEGER,

"lte_se" INTEGER,

PRIMARY KEY ("id") ,

CONSTRAINT "fk_ap_report_info_ap_report_info_1" FOREIGN KEY ("ap_mac") REFERENCES "ap_info" ("ap_mac"),

CONSTRAINT "locate_area" UNIQUE ("ap_mac")

);



