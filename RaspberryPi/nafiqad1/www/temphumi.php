<?php
require("config/lang.ini.php");
require("config/config.php"); 
$serial=addslashes($_GET['serial']);
$temp=addslashes($_GET['temp']);
$humi=addslashes($_GET['humi']);
$type=substr($serial,0,3)=="THS";
$deviceid=GetDeviceIdFromSerial($serial);
if(CheckExistDeviceSerial($serial)){
	if($type=='THS'){
		UpdateTempValue($serial,$temp);
		UpdateHumiValue($serial,$humi);
		if(CheckHumiInRange($serial,$humi)||CheckTempInRange($serial,$temp)){
			if(ReadConfigByName('alert')){	
				$warning_content=GetDeviceNameFromId($deviceid).'\nNhiet do: '.$temp.RangeTempToString($serial).'\nDo am: '.$humi.RangeHumiToString($serial);
				UpdateWarningLog($deviceid,$warning_content);
			}
			if(ReadConfigByName('sms')&&(!ReadConfigByName('bell'))){
				SetConfig('sms_pending',1);
			}
			if(ReadConfigByName('email')&&(!ReadConfigByName('bell'))){
				SetConfig('email_pending',1);
			}
			if(ReadConfigByName('ring')){	SetConfig('bell',1);}
			echo ">>alert==ON&&";
		}else{
			echo ">>alert==OFF&&";
		}
	}
}
?>