<?php 
require("../config/lang.ini.php");
require("../config/config.php"); 
session_start();
if (!$_SESSION['user'])
{ 
?>
<ion-view view-title="<?php echo __add_device?>" name="device_add-tab">
	<ion-content class="padding">
    <?php echo __you_dont_permission?>
    </ion-content>
</ion-view>
<?php
} else{
?>
<ion-view class="padding" view-title="<?php echo __add_device?>" name="device_add-tab">
	<ion-list>
	<form name="add" ng-submit="AddDevice()" class="list-inset" novalidate>
		<label class="item item-input item-floating-label">
		<span class="input-label"><?php echo __serial?></span>
		<input type="text" ng-model="serial" placeholder="<?php echo __serial?>" required ng-minlength='10'>
		</label>
		<label class="item item-input item-floating-label" >
		<span class="input-label"><?php echo __name?></span>
		<input type="text" ng-model="name" placeholder="<?php echo __name?>" required ng-minlength='5'>
		</label>
		<button type="submit" class="button button-balanced button-block" ng-disabled="add.$invalid"><?php echo __add_device?></button>
	</form>
	</ion-list>
</ion-view>
<?php }?>