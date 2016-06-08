<?php 
require("../config/lang.ini.php");
require("../config/config.php"); 
session_start();
if (!$_SESSION['user'])
{ 
    header("Location: login.tpl.php"); 
} else{
?>
<ion-view view-title="<?php echo __device?>" name="device-view">
	<ion-content class="padding">
  	<ion-list>
		<ion-item class="item-icon-left" href="#/tab/device.add"><i class="icon ion-plus-round"></i><?php echo __add_device?></ion-item>
		<ion-item class="item-icon-left" href="#/tab/device.config"><i class="icon ion-ios-cog"></i><?php echo __config_device?></ion-item>
	<ion-list>
  </ion-content>
</ion-view>
<?php }?>