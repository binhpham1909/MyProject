<?php 
require("config/lang.ini.php");
require("config/config.php"); 
session_start();
if ($_SESSION['user'])
{ 
    header("Location: index.php"); 
} else{
?>
<!DOCTYPE html>
<html ng-app="hbiot">
<head>
    <meta charset="utf-8">
    <title><?php echo __login?></title>
    <meta name="viewport" content="initial-scale=1, maximum-scale=1, user-scalable=no, width=device-width">
    <link rel="stylesheet" href="lib/css/ionic.min.css" />
    <link rel="stylesheet" href="lib/css/ionicons.min.css" />
    <link rel="stylesheet" href="lib/css/style.css" />
    <script src="lib/js/ionic.bundle.min.js"></script>
    <script src="js/login.js"></script>
</head>
<body>
<form name="user" ng-submit="login()" ng-controller="login" class="list-inset" novalidate>
	<label class="item item-input" ng-class="{'has-errors':user.username.$invalid&&!user.username.$pristine,'no-errors':user.username.$valid}">
		<span class="input-label"><?php echo __username?></span>
		<input name="username" type="text" placeholder="<?php echo __username?>" ng-model="username" required ng-minlength='2' ng-maxlength='32'>
	</label>
	<label class="item item-input" ng-class="{ 'has-errors' : user.password.$invalid && !user.password.$pristine, 'no-errors' : user.password.$valid}">
		<span class="input-label"><?php echo __password?></span>
		<input name="password" type="password" placeholder="<?php echo __password?>" ng-model="password" required ng-minlength='2' ng-maxlength='32'>
	</label>
    <span class="">{{status}}</span>
	<button type="submit" class="button button-balanced button-block" ng-disabled="user.$invalid"><?php echo __login?></button>
</form>

</body>
</html>
<?php }?>