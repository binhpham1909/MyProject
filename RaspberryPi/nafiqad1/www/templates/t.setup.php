<?php 
require("../config/lang.ini.php");
require("../config/config.php"); 
session_start();
if (!$_SESSION['user'])
{ 
    header("Location: login.tpl.php"); 
} else{
?>
<ion-view view-title="<?php echo __status?>" name="setup-view">
	<ion-content class="padding">
  	<ion-list class="">
		<ion-item class="item-icon-left" href="#/tab/setup.temp"><i class="icon ion-thermometer"></i><?php echo __temp?></ion-item>
		<ion-item class="item-icon-left" href="#/tab/setup.humi"><i class="icon ion-umbrella"></i><?php echo __humi?></ion-item>
	<ion-list>
  </ion-content>
</ion-view>
<?php }?>