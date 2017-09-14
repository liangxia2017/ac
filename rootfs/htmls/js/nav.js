var outlookbar = new outlook();
var t;
t = outlookbar.addtitle('基本配置', 1, 1);
outlookbar.additem('AC版本信息', t, 'ac/ac_config/basic_config/ac_version_info/ac_version_info.php');
outlookbar.additem('网络配置', t, 'ac/ac_config/basic_config/ac_network_config/ac_network_config.php');
outlookbar.additem('账号管理', t, 'ac/ac_config/basic_config/ac_user_info/ac_user_info.php');
outlookbar.additem('基本配置',t,'ac/ac_config/basic_config/ac_basic_config/ac_basic_config.php');
t = outlookbar.addtitle('高级配置', 1, 1);
outlookbar.additem('DHCP服务器', t, 'ac/ac_config/advance_config/ac_dhcp_server/ac_dhcp_server.php');
outlookbar.additem('定位功能', t, 'ac/ac_config/advance_config/ap_locate_edit/ap_locate_edit.php');
outlookbar.additem('AC升级及还原', t, 'ac/ac_config/advance_config/ac_upgrade/ac_upgrade.php');

t = outlookbar.addtitle('AP分组', 2, 1);
outlookbar.additem('AP组配置', t, 'ac/group_config/ap_group/ap_group_config/ap_group_config.php');
//outlookbar.additem('AP信息注册', t, 'ac/group_config/ap_group/ap_version/ap_version.php');
outlookbar.additem('AP版本导入', t, 'ac/group_config/ap_group/ap_upgrade_config/ap_upgrade_config.php');

t = outlookbar.addtitle('WLAN分组', 2, 1);
outlookbar.additem('WLAN组配置', t, 'ac/group_config/wlan_group/wlan_group/wlan_group.php');
outlookbar.additem('安全策略分组', t, 'ac/group_config/wlan_group/wlan_security_policy/wlan_security_policy.php');
t = outlookbar.addtitle('无线分组', 2, 1);
outlookbar.additem('无线组配置', t, 'ac/group_config/wireless_group/wireless_group/wireless_group.php');
t = outlookbar.addtitle('功能分组', 2, 1);
outlookbar.additem('功能组配置', t, 'ac/group_config/function_group/function_group/function_group.php');
t = outlookbar.addtitle('分组关联策略', 2, 1);
outlookbar.additem('分组关联策略', t, 'ac/group_config/group_relation/group_relation/group_relation.php');

t = outlookbar.addtitle('统计信息', 3, 1);
outlookbar.additem('关联终端信息表', t, 'ac/sta_info/sta_info/sta_list_assc.php');
outlookbar.additem('定位终端信息表', t, 'ac/sta_info/sta_info/sta_list_locate.php');
outlookbar.additem('关联终端黑名单', t, 'ac/sta_info/sta_info/sta_blacklist.php');
outlookbar.additem('设备网管统计表', t, 'ac/sta_info/sta_snmp_info/ap_report_total.php');