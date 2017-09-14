<?php 
//include 'dbhelper.php';
//include 'page.php';
//$countSql='select count(*) from users';
//$selectSql='select * from users';
//$page = new Page($selectSql,3,'/index.php','select count(*) from users');

?>
<!--<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />-->
<span class="STYLE22">共有
<strong><font color="red"><?php echo $page->getCount(); ?></font></strong>
条记录&nbsp;&nbsp;当前
<strong><font color="red"><?php echo $page->getPageNow(); ?></font>/
<font color="red"><?php echo $page->getPageCount(); ?></font></strong>
页</span>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<?php 
	if($page->getPageNow()<=1){
		echo "<img align='absbottom' src='".PATH."images/main_54.gif'>";
	}else{
		echo "<a href='#' onclick='page_forward(1)'><img align='absbottom' src='".PATH."images/main_54.gif'  style='border:0'></a>";
	}
	echo "&nbsp;&nbsp;";
	if($page->getPageNow()==1){
		echo "<img align='absbottom' src='".PATH."images/main_56.gif'>";
	}else{
		echo "<a href='#' onclick='page_forward(".($page->getPageNow()-1).")'><img align='absbottom' src='".PATH."images/main_56.gif'  style='border:0'></a>";
	}
	echo "&nbsp;&nbsp;";
	if($page->getPageNow()>=$page->getPageCount()){
		echo "<img align='absbottom' src='".PATH."images/main_58.gif'>";
	}else{
		echo "<a href='#' onclick='page_forward(".($page->getPageNow()+1).")'><img align='absbottom' src='".PATH."images/main_58.gif'  style='border:0'></a>";
	}
	echo "&nbsp;&nbsp;";
	if($page->getPageNow()>=$page->getPageCount()){
		echo "<img align='absbottom' src='".PATH."images/main_60.gif'>";
	}else{
		echo "<a href='#' onclick='page_forward(".($page->getPageCount()).")'><img align='absbottom' src='".PATH."images/main_60.gif'  style='border:0'></a>";
	}
	echo "&nbsp;&nbsp;";
?>
<span class="STYLE22">转到第<input
	onkeypress='if (event.keyCode==13){return onInputKeyPress(this.value);}'
	onKeyUp="value=value.replace(/[^\d]/g,'')" type=text size='2' value='<?php echo $page->getPageNow()?>'
	id='pageNow' name='pageNow'
	style='width: 20px; height: 12px; font-size: 12px; border: solid 1px #7aaebd;' />页</span>
&nbsp;
<a href='#' onclick="submitPageForm()"><img align="absbottom"  src="<?php echo PATH?>images/main_62.gif" style="border: 0"/></a>
<script type='text/javascript'>
function page_forward(startPage){
	$('#pageNow').val(startPage);
	submitPageForm();
}
function submitPageForm(){
	if($('#pageNow').val()==''){
		return false;
	}
	$('#<?php echo $formId?>').submit();
	return true;
}
function onInputKeyPress(c){
	var reg =/^\d+$/; 
	if(!reg.test(c)){
		return false;
	}
	submitPageForm();
	return true;
}
</script>
