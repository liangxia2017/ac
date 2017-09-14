<?php
//unset($_SESSION['USER']);
session_start();
session_unset();
//$_SESSION = array();
echo "<script>location='login.php';</script>";
?>