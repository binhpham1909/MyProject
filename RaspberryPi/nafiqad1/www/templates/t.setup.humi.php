<?php 
require("../config/lang.ini.php");
require("../config/config.php"); 
session_start();
if (!$_SESSION['user'])
{ 
?>
<ion-view view-title="<?php echo __humi?>" name="setup_humi-tab">
	<ion-content class="padding">
    <?php echo __you_dont_permission?>
    </ion-content>
</ion-view>
<?php
} else{
?>
<ion-view view-title="<?php echo __humi?>" name="setup_humi-tab">
<ion-nav-buttons side="right">
	<button class="button button-positive button-clear icon-left ion-edit" ng-click="Edit()"></button>
</ion-nav-buttons>
<ion-content>
	<ion-list show-delete="showRename" show-reorder="showEdit">
        <ion-item ng-repeat="humi in humis" class="item-remove-animate" on-hold="visRename()">
		<h2>{{humi.name}}</h2> <?php echo __limit_min?> : {{humi.min_humi}} - <?php echo __limit_max?> : {{humi.max_humi}}
		<ion-delete-button class="ion-ios-compose-outline" ng-click="rename(humi.deviceid,humi.name)"></ion-delete-button>
		<ion-reorder-button class="ion-edit" ng-click="changerange(humi.id,humi.name,humi.min_humi,humi.max_humi)"></ion-reorder-button>
		</ion-item>
	</ion-list>
</ion-content>
</ion-view>
<?php }?>