<?php 
require("../config/lang.ini.php");
require("../config/config.php"); 
session_start();
if (!$_SESSION['user'])
{ 
?>
<ion-view view-title="<?php echo __humi?>" name="setup_temp-tab">
	<ion-content class="padding">
    <?php echo __you_dont_permission?>
    </ion-content>
</ion-view>
<?php
} else{
?>
<ion-view view-title="<?php echo __humi?>" name="setup_temp-tab">
<ion-nav-buttons side="right">
	<button class="button button-positive button-clear icon-left ion-edit" ng-click="Edit()"></button>
</ion-nav-buttons>
<ion-content>
	<ion-list show-delete="showRename" show-reorder="showEdit">
        <ion-item ng-repeat="temp in temps" class="item-remove-animate" on-hold="visRename()">
		<h2>{{temp.name}}</h2> <?php echo __limit_min?> : {{temp.min_temp}} - <?php echo __limit_max?> : {{temp.max_temp}}
		<ion-delete-button class="ion-ios-compose-outline" ng-click="rename(temp.deviceid,temp.name)"></ion-delete-button>
		<ion-reorder-button class="ion-edit" ng-click="changerange(temp.id,temp.name,temp.min_temp,temp.max_temp)"></ion-reorder-button>
		</ion-item>
	</ion-list>
</ion-content>
</ion-view>
<?php }?>