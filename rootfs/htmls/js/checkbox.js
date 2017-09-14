	/**
	 * 
	 * @param t 当前点击的对象
	 * @param tag 要操作checkbox的name
	 */
	function select_all(t,tag) {
				var arrObj = document.getElementsByName(tag);
				if(t.checked==false){
					for ( var i = 0; i < arrObj.length; i++) {
						arrObj[i].checked=false;
					}
				}else{
					for ( var i = 0; i < arrObj.length; i++) {
						arrObj[i].checked=true;
					}
				}
			}
	
	/**
	 * 
	 * @param t 当前点击的对象
	 * @param tag 要操作checkbox的name
	 * @param action 提交到的action(url)
	 */
	function delete_all(t,tag,action){
		var arrObj = document.getElementsByName(tag);
		var count = 0;
		for ( var i = 0; i < arrObj.length; i++) {
			if(arrObj[i].checked==true){
				count++;
			}
		}
		if(count==0){
			alert("你还没有选择要删除的项!");
		}else{
			if(confirm("你确定删除这些数据!")){  //"/admin.do?action=delete"
				t.form.action=action;				
				t.form.submit();
			}
		}
	}		
	/**
	 * 
	 * @param cid checkbox id
	 * @param fid form id
	 * @param url  submit url
	 */
	function opt_all(cid,fid,url){
		var all = $("input[id='"+cid+"']");
		var count = 0;
		for ( var i = 0; i < all.length; i++) {
			if(all[i].checked==true){
				count++;
			}
		}
		if(count==0){
			alert("你还没有选择要操作的项!");
		}else{
			if(confirm("你确定要操作这些数据吗?")){
				$("#"+fid).attr("action",url);
				$("#"+fid).submit();
			}
		}
	}