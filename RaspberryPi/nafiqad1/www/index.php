<?php 
require("config/lang.ini.php");
require("config/config.php"); 
session_start();
$header_title="Báo cáo tiêu hao";
?>
<!DOCTYPE html>
<html ng-app="hbiot">
<head>
    <meta charset="utf-8">
    <title><?php echo __tiltle?></title>
    <meta name="viewport" content="initial-scale=1, maximum-scale=1, user-scalable=no, width=device-width">
    <link rel="stylesheet" href="lib/css/ionic.min.css" />
    <link rel="stylesheet" href="lib/css/ionicons.min.css" />
    <link rel="stylesheet" href="lib/css/style.css" />
    <script src="lib/js/ionic.bundle.min.js"></script>
	<script src="js/service.js"></script>
    <script src="js/tabs.js"></script>
    <script src="js/status.js"></script>
	<script src="js/setup.js"></script>
	<script src="js/device.js"></script>
	
</head>
<body>
<ion-nav-bar class="bar-light">
	<ion-nav-back-button>
    </ion-nav-back-button>
</ion-nav-bar>
<ion-nav-view></ion-nav-view>

<script id="templates/tabs.html" type="text/ng-template">
	<ion-tabs class="tabs-icon-top tabs-light">
	<!--Control tab-->
		<ion-tab title="<?php echo __status?>" icon-on="ion-ios-information positive" icon-off="ion-ios-information-outline positive" href="#/tab/status">
			<ion-nav-view name="status-tab"></ion-nav-view>
		</ion-tab>
		<ion-tab title="<?php echo __setup?>" icon-on="ion-ios-gear positive" icon-off="ion-ios-gear-outline positive" href="#/tab/setup">
			<ion-nav-view name="setup-tab"></ion-nav-view>
		</ion-tab>
		<ion-tab title="<?php echo __device?>" icon-on="ion-ios-person positive" icon-off="ion-ios-person-outline positive" href="#/tab/device">
			<ion-nav-view name="device-tab"></ion-nav-view>
		</ion-tab>
	</ion-tabs>
</script>

</body>

</html>