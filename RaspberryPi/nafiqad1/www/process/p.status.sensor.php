<?php
require("../config/config.php");
 
$postdata = file_get_contents("php://input");
$request = json_decode($postdata);
$sensor=$request->sensor;

$reply = array();
$sql1="";
if($sensor=="temp"){
	$sql1="SELECT * FROM sensor_temp ORDER BY id ASC";
	$query1 = mysql_query($sql1);
	while ($row1 = mysql_fetch_assoc($query1)){
		$alert=0;
		if(($row1['temp']>$row1['max_temp'])||($row1['temp']<$row1['min_temp'])){$alert=1;}
		$arr=array(
			"id"=>$row1['id'],
			"deviceid"=>$row1['device'],
			"name"=>GetDeviceName($row1['device']),
			"serial"=>GetDeviceSerial($row1['device']),
			"temp"=>$row1['temp'],
			"min_temp"=>$row1['min_temp'],
			"max_temp"=>$row1['max_temp'],
			"alert"=>$alert
		);
		array_push($reply,$arr);
	}
}elseif($sensor=="humi"){
	$sql1="SELECT * FROM sensor_humi ORDER BY id ASC";
	$query1 = mysql_query($sql1);
	while ($row1 = mysql_fetch_assoc($query1)){
		$alert=0;
		if(($row1['humi']>$row1['max_humi'])||($row1['humi']<$row1['min_humi'])){$alert=1;}
		$arr=array(
			"id"=>$row1['id'],
			"deviceid"=>$row1['device'],
			"name"=>GetDeviceName($row1['device']),
			"serial"=>GetDeviceSerial($row1['device']),
			"humi"=>$row1['humi'],
			"min_humi"=>$row1['min_humi'],
			"max_humi"=>$row1['max_humi'],
			"alert"=>$alert
		);
		array_push($reply,$arr);
	}
}
echo json_encode($reply);
?>