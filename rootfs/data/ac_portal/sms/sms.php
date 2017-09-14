<?php
/*
 *  Copyright (c) 2014 The CCP project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a Beijing Speedtong Information Technology Co.,Ltd license
 *  that can be found in the LICENSE file in the root of the web site.
 *
 *   http://www.yuntongxun.com
 *
 *  An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */
date_default_timezone_set("Asia/Shanghai"); //设置时区
include_once("./CCPRestSmsSDK.php");

//主帐号,对应开官网发者主账号下的 ACCOUNT SID
//$accountSid= '8a48b5514bde8c71014be8a9fc17069e';
$accountSid= '8a48b5514d32a2a8014d89b7042d3dd0';

//主帐号令牌,对应官网开发者主账号下的 AUTH TOKEN
//$accountToken= 'e66e436748834a08b58d5f791fbfd744';
$accountToken= '3b2c739323fa439eb5b49552fdfc1ac2';

//应用Id，在官网应用列表中点击应用，对应应用详情中的APP ID
//在开发调试的时候，可以使用官网自动为您分配的测试Demo的APP ID
//$appId='aaf98f894bde8b95014be8bb750e0610';
$appId='8a48b5514d32a2a8014d8a25b4a73e8d';

//请求地址
//沙盒环境（用于应用开发调试）：sandboxapp.cloopen.com
//生产环境（用户应用上线使用）：app.cloopen.com
//$serverIP='sandboxapp.cloopen.com';
$serverIP='app.cloopen.com';

//请求端口，生产环境和沙盒环境一致
$serverPort='8883';

//REST版本号，在官网文档REST介绍中获得。
$softVersion='2013-12-26';


/**
  * 发送模板短信
  * @param to 手机号码集合,用英文逗号分开
  * @param datas 内容数据 格式为数组 例如：array('Marry','Alon')，如不需替换请填 null
  * @param $tempId 模板Id,测试应用和未上线应用使用测试模板请填写1，正式应用上线后填写已申请审核通过的模板ID
  */       
function sendTemplateSMS($to,$datas,$tempId)
{
     // 初始化REST SDK
     global $accountSid,$accountToken,$appId,$serverIP,$serverPort,$softVersion;
     $rest = new REST($serverIP,$serverPort,$softVersion);
     $rest->setAccount($accountSid,$accountToken);
     $rest->setAppId($appId);
    
     // 发送模板短信
     $result = $rest->sendTemplateSMS($to,$datas,$tempId);
     if($result == NULL ) {
         //echo "result error!";
         echo 1;
         break;
     }
     if($result->statusCode!=0) {
         //echo "error code :" . $result->statusCode . "<br>";
         //echo "error msg :" . $result->statusMsg . "<br>";
         //TODO 添加错误处理逻辑
         echo 1;
     }else{/**
         echo "Sendind TemplateSMS success!<br/>";
         // 获取返回信息
         $smsmessage = $result->TemplateSMS;
         echo "dateCreated:".$smsmessage->dateCreated."<br/>";
         echo "smsMessageSid:".$smsmessage->smsMessageSid."<br/>";
         //TODO 添加成功处理逻辑
         echo "<form action='SMS.php' method='POST'><input type='hidden' name='num' value='".md5($datas[0])."' /><input type='submit' value='0000' /></form>";
         **/
         //setcookie("code",md5($datas[0]));
         session_start();
         $_SESSION["code"] = $datas[0];
         $_SESSION["tel"] = $to;
         //setcookie("tel",$to);
         echo 0;
     }
}

//Demo调用 
		//**************************************举例说明***********************************************************************
		//*假设您用测试Demo的APP ID，则需使用默认模板ID 1，发送手机号是13800000000，传入参数为6532和5，则调用方式为           *
		//*result = sendTemplateSMS("13800000000" ,array('6532','5'),"1");																		  *
		//*则13800000000手机号收到的短信内容是：【云通讯】您使用的是云通讯短信模板，您的验证码是6532，请于5分钟内正确输入     *
		//*********************************************************************************************************************
//sendTemplateSMS("18826558761",array('123456789','5'),"1");//手机号码，替换内容数组，模板ID
$to = $_GET["tel"];
$time = $_GET["time"];
//$to = "18826558761";
$num = rand(0,9).rand(0,9).rand(0,9).rand(0,9);
$arr = array($num,"1");
sendTemplateSMS($to,$arr,21155);
?>
