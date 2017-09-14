<html>
<?php
include "db/dbhelper.php";
$dbhelper = new DAL();



$path = "jingpin/";
if(isset($_POST["title"]) && $_POST["title"] != ""){
    $file = $_FILES["img"];
//    var_dump($file);
    if(move_uploaded_file($file["tmp_name"],$path.$file["name"])){
        $params = array($file["name"],$_POST["title"],$_POST["intro"],$_POST["shop"],$_POST["tel"]);
        $sql = "insert into jingpin(path,keys,intro,shop,tel) values (?,?,?,?,?)";
        $insert=$dbhelper->insert($sql,$params);
        if($insert > 0){
            echo "<script>alert('success!');</script>";
        }else{
            echo "<script>alert('faile insert!');</script>";
        }
    }else{
        echo "<script>alert('faile move!');</script>";
    }
}
$num = $dbhelper->getOne("select count(*) from jingpin");
?>
<body>
<form method="post" enctype="multipart/form-data">
    <div>num  :<input type="text" disabled="disabled" value="<?php echo $num;?>" /></div>
    <div>photo:<input type="file" name="img" /></div>
    <div>name :<input type="text" name="title" /></div>
    <div>intro:<textarea name="intro" ></textarea></div>
    <div>shop :<input type="text" name="shop" /></div>
    <div>tel  :<input type="text" name="tel" /></div>
    <input type="submit" value="submit" />
</form>
</body>
</html>