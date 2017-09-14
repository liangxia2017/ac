<?php
if(isset($_POST["num"])){
    setcookie("num",$_POST["num"]);
}
?>
<html>
<body>
<form action="validate.php" method="POST" name="fo">
<input type="text" name="ran" id="ran" oninput="vali(this)" />
<input type="text" value="" id="tes" />
<input type="submit" id="sub" disabled="disabled" />
</form>
</body>
<script>
function vali(t){
    var sub = document.getElementById("sub");
    var tes = document.getElementById("tes");
    tes.value = t.value;
    if((t.value).trim().length > 0){
        //sub.removeAttribute("disabled");
        sub.disabled = false;
    }else{
        sub.disabled = true;
    }
}
</script>
</html>