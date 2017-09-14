CREATE TRIGGER [ON_TBL_AP_GROUP_DELETE]
AFTER DELETE ON [ap_group]
FOR EACH ROW
BEGIN
update ap_info set ap_group_name='unknown' where ap_group_name=old.ap_group_name;
delete from group_relation where ap_group_name=old.ap_group_name;
delete from sta_blacklist where ap_group_name=old.ap_group_name;
END;

CREATE TRIGGER [ON_TBL_AP_GROUP_INSERT]
AFTER INSERT ON [ap_group]
FOR EACH ROW
BEGIN
insert into group_relation(ap_group_name) values(new.ap_group_name);
END;

CREATE TRIGGER [ON_TBL_AP_GROUP_UPDATE]
AFTER UPDATE ON [ap_group]
FOR EACH ROW
BEGIN
update ap_info set ap_group_name=new.ap_group_name where ap_group_name=old.ap_group_name;
update group_relation set ap_group_name=new.ap_group_name where ap_group_name=old.ap_group_name;
update sta_blacklist set ap_group_name=new.ap_group_name where ap_group_name=old.ap_group_name;
END;

CREATE TRIGGER [ON_TBL_FUNCTION_GROUP_DELETE]
AFTER DELETE ON [function_group]
FOR EACH ROW
BEGIN
delete from func_config where function_group_name=old.function_group_name;
update group_relation set function_group_name='' where function_group_name=old.function_group_name;
END;

CREATE TRIGGER [ON_TBL_FUNCTION_GROUP_UPDATE]
AFTER UPDATE ON [function_group]
FOR EACH ROW
BEGIN
update func_config set function_group_name=new.function_group_name where function_group_name=old.function_group_name;
update group_relation set function_group_name=new.function_group_name where function_group_name=old.function_group_name;
END;

CREATE TRIGGER [ON_TBL_WIRELESS_GROUP_DELETE]
AFTER DELETE ON [wireless_group]
FOR EACH ROW
BEGIN
delete from wireless_config where wireless_group_name=old.wireless_group_name;
update group_relation set wireless_group_name='' where wireless_group_name=old.wireless_group_name;
END;

CREATE TRIGGER [ON_TBL_WIRELESS_GROUP_UPDATE]
AFTER UPDATE ON [wireless_group]
FOR EACH ROW
BEGIN
update wireless_config set wireless_group_name=new.wireless_group_name where wireless_group_name=old.wireless_group_name;
update group_relation set wireless_group_name=new.wireless_group_name where wireless_group_name=old.wireless_group_name;
END;

CREATE TRIGGER [ON_TBL_WLAN_GROUP_DELETE]
AFTER DELETE ON [wlan_group]
FOR EACH ROW
BEGIN
delete from wlan_config where wlan_group_name=old.wlan_group_name;
update group_relation set wlan_group_name='' where wlan_group_name=old.wlan_group_name;
END;

CREATE TRIGGER [ON_TBL_WLAN_GROUP_UPDATE]
AFTER UPDATE ON [wlan_group]
FOR EACH ROW
BEGIN
update wlan_config set wlan_group_name=new.wlan_group_name where wlan_group_name=old.wlan_group_name;
update group_relation set wlan_group_name=new.wlan_group_name where wlan_group_name=old.wlan_group_name;
END;