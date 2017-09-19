include	config.grentech
SOFTWARE_VER=AC160928
DB_VER=160928

#export SOFTWARE_VER="AC`date -d today +"%y%m%d"`"
#DB_VER=`date -d today +"%y%m%d"`
IMG_KEY=guoren_ac

#export LIBRARY_PATH=$(GR_INSTALL_ROOT)/lib/:$LIBRARY_PATH

all:	image_pre sqlite_build openssl_build dhcpd_build	sshpass_build\
locating_build bftpd_build capwapd_build php_build nginx_build webman_build crypt_build read_block_build \
 write_block_build dhcpd_pool_build ntp_build img_build portal_img_build
 
php_build:
	cd ../apps/$(LIBXML) && ./configure --prefix=$(GR_INSTALL_ROOT)/libxml CFLAGS=-Os && make && make install && make clean;
	cd ../apps/php-5.4.45 && ./configure --prefix=$(GR_INSTALL_ROOT)/php --with-openssl=$(GR_INSTALL_ROOT)/openssl --with-libxml-dir=$(GR_INSTALL_ROOT)/libxml \
	--enable-sockets --with-curl --with-config-file-path=$(GR_INSTALL_ROOT)/php/lib --enable-fpm CFLAGS=-Os \
	&& make && make install && cp -f ../php_conf_guoren/* $(GR_INSTALL_ROOT)/php/etc && make clean
	
php_clean:
	cd ../apps/$(LIBXML) && make clean ;
	cd ../apps/php-5.4.45 && make clean ;
	
nginx_build:
	cd ../apps/nginx-1.8.1 && ./buildconf --force && ./configure --with-http_ssl_module --with-pcre=../pcre-8.35 \
  --with-openssl=../openssl-1.0.1j/ --prefix=$(GR_INSTALL_ROOT)/nginx --with-cc-opt=-Os && make && make install && \
  cp guoren/* $(GR_INSTALL_ROOT)/nginx/conf && make clean
  
nginx_clean:
	cd ../apps/nginx-1.8.1 && make clean;
 
sshpass_build:
	cd ../apps/sshpass-1.05 && ./configure --prefix=$(GR_INSTALL_ROOT)/sshpass && make && make install && make clean
sshpass_clean:
	cd ../apps/dhcpd-pools-2.25 && make clean;
 
dhcpd_pool_build:
	cd ../apps/dhcpd-pools-2.25 && make clean && ./configure --prefix=$(GR_INSTALL_ROOT)/dhcpd_pool --with-uthash=./lib/ CFLAGS=-DGNULIB_defined_struct_option && make && make install && make clean
	
dhcpd_pool_clean:
	cd ../apps/dhcpd-pools-2.25 && make clean;
 
ntp_build:
	cd ../apps/$(NTP) && ./configure --prefix=$(GR_INSTALL_ROOT)/ntp && make && make install && make clean
	
ntp_clean:
	cd ../apps/$(NTP) && make clean;
 
license_tool_build:
	cd ../apps/license_tool && make;
	
license_tool_clean:
	cd ../apps/license_tool && make clean;
	
read_block_build:
	cd ../apps/read_block && make && cp -f read_block $(GR_INSTALL_ROOT)/sbin/ ;
	
read_block_clean:
	cd ../apps/read_block && make clean;
	
write_block_build:
	cd ../apps/write_block && make && cp -f write_block $(GR_INSTALL_ROOT)/sbin/ ;
	
write_block_clean:
	cd ../apps/write_block && make clean;
	
crypt_build:
	cd ../apps/crypt && make && cp -f crypt $(GR_INSTALL_ROOT)/sbin/ ;

crypt_clean:
	cd ../apps/crypt && make clean
	
webman_build:
	cd ../apps/$(WEBTOOLS)/cgi-bin/library; $(MAKE) ; \
	cp libwebman.a $(GR_INSTALL_ROOT)/lib/ ;\

webman_clean:
	cd ../apps/$(WEBTOOLS)/cgi-bin/library && make clean  ; 
			
sqlite_build:
	cd ../apps/$(SQLITE) && ./configure --prefix=$(GR_INSTALL_ROOT)/sqlite && make && make install && make clean
	
sqlite_clean:
	cd ../apps/$(SQLITE) && make clean
			
capwapd_build:
		cd ../apps/$(CAPWAPD) && make && cp -f capwapd $(GR_INSTALL_ROOT)/sbin;
capwapd_clean:
		cd ../apps/$(CAPWAPD) && make clean 			

httpd_build:
	cd ../apps/$(APACHE) && ./configure --prefix=$(GR_INSTALL_ROOT)/apache --with-ssl=$(GR_INSTALL_ROOT)/openssl LDFLAGS="-Wl,--rpath -Wl,$(GR_INSTALL_ROOT)/openssl/lib" \
	--enable-module=so --enable-so --enable-ssl CFLAGS=-DBIG_SECURITY_HOLE && make && make install
	cd ../apps/$(LIBXML) && ./configure --prefix=$(GR_INSTALL_ROOT)/libxml && make && make install
	cd ../apps/$(PHP) && ./configure --prefix=$(GR_INSTALL_ROOT)/php --with-openssl=$(GR_INSTALL_ROOT)/openssl --with-apxs2=$(GR_INSTALL_ROOT)/apache/bin/apxs --with-libxml-dir=$(GR_INSTALL_ROOT)/libxml \
	--enable-pdo --with-sqlite --with-pdo-sqlite --enable-sockets --enable-sqlite-utf8  --with-curl \
	--with-config-file-path=$(GR_INSTALL_ROOT)/php/lib --enable-shared --cache-file=/dev/null\
	 && make && make install

httpd_install:httpd_build
	cp -fR ../apps/$(APACHE)/guoren/* $(GR_INSTALL_ROOT)/apache/conf/
	cp -fR ../apps/$(APACHE)/guoren/httpd-ssl.conf $(GR_INSTALL_ROOT)/apache/conf/extra
	cp -fR ../apps/$(APACHE)/guoren/httpd-vhosts.conf $(GR_INSTALL_ROOT)/apache/conf/extra
	cp -fR ../apps/$(APACHE)/guoren/httpd-mpm.conf $(GR_INSTALL_ROOT)/apache/conf/extra
	cp -fR ../apps/$(PHP)/guoren/php.ini $(GR_INSTALL_ROOT)/php/lib

httpd_clean:
	cd ../apps/$(APACHE) && make clean 

dhcpd_build:
	cd ../apps/$(DHCPD) && ./configure --prefix=$(GR_INSTALL_ROOT)/dhcpd --enable-dhcpv6 && make && make install && make clean
	
dhcpd_clean:
	cd ../apps/$(DHCPD) && make clean 
	
openssl_build:
	cd ../apps/$(OPENSSL)/ && make clean && ./config shared -fPIC --prefix=$(GR_INSTALL_ROOT)/openssl && make && make install
	
openssl_clean:
	cd ../apps/$(OPENSSL)/ && make clean ;
	
bftpd_build:
	cd ../apps/$(BFTPD)/ && mkdir -p $(GR_INSTALL_ROOT)/bftpd/sbin $(GR_INSTALL_ROOT)/bftpd/man && \
	./configure --prefix=$(GR_INSTALL_ROOT)/bftpd &&make && make install && \
	cp -f bftpd.conf $(GR_INSTALL_ROOT)/bftpd && cp -f .u $(GR_INSTALL_ROOT)/bftpd/man
bftpd_clean:
	cd ../apps/$(BFTPD)/ && make clean
	
image_pre:
	rm -rf $(GR_INSTALL_ROOT)/;
	chmod 777 -R ../*;
	mkdir -p $(GR_INSTALL_ROOT)/;
	cp -rf *.sql  $(GR_INSTALL_ROOT)/../rootfs/config/;
	cp -rf $(GR_INSTALL_ROOT)/../rootfs/* $(GR_INSTALL_ROOT)/;
img_build:
	chmod 777 -R ../*;
	/opt/micro_ac/apps/write_block/write_block -v $(SOFTWARE_VER)
	/opt/micro_ac/apps/write_block/write_block -d $(DB_VER)
	ln -sf -T /opt/micro_ac/runtime/ /ac
	rm -rf /opt/micro_ac/runtime/db/*
	/opt/micro_ac/runtime/script/init_sql_db
	mv -f /opt/micro_ac/runtime/data /tmp/
	mkdir -p /opt/micro_ac/runtime/data/ap_img
	mkdir -p /opt/micro_ac/runtime/data/ac_portal
	cd /opt/micro_ac && rm -f $(SOFTWARE_VER)_$(DB_VER).img && tar -czvf runtime.tmp runtime;
	rm -rf /opt/micro_ac/runtime/data/;
	mv -f /tmp/data /opt/micro_ac/runtime/ ;
	/opt/micro_ac/apps/crypt/crypt -q -K $(IMG_KEY) -f /opt/micro_ac/runtime.tmp -o /opt/micro_ac/$(SOFTWARE_VER)_$(DB_VER).img;
	rm -rf /opt/micro_ac/*.tmp ;
	
portal_img_build:
	rm -f /opt/micro_ac/ac_portal.img && rm -f /ac/data/ac_portal/db/ac_portal.s3db;
	/opt/micro_ac/runtime/script/init_sql_db
	cd /opt/micro_ac/runtime/data/ && tar -czvf /opt/micro_ac/ac_portal.img ac_portal
	
locating_build:
	cd ../apps/locating && make && cp -f locating $(GR_INSTALL_ROOT)/sbin	
locating_clean:
	cd ../apps/locating && make clean
	
clean:sqlite_clean webman_clean dhcpd_clean locating_clean \
capwapd_clean openssl_clean sshpass_clean bftpd_clean php_clean nginx_clean crypt_clean read_block_clean \
write_block_clean dhcpd_pool_clean ntp_clean
 


