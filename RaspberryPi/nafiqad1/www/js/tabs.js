// Include dependency: ngCordova
myapp=angular.module('hbiot', ['ionic','services']);

myapp.config(function($stateProvider, $urlRouterProvider){
	$stateProvider
	// Master tabs
    .state('tabs', {
      url: "/tab",
      abstract: true,
      templateUrl: "templates/tabs.html"
    })

	///////////////////////
	//    Tab trang thai   //
	///////////////////////

    .state('tabs.status', {
      url: "/status",
      views: {
        'status-tab': {
          templateUrl: "templates/t.status.php",
          controller: 'status'
        }
      }
    })
    .state('tabs.status_temp', {
      url: "/status.temp",
      views: {
        'status-tab': {
          templateUrl: "templates/t.status.temp.php",
          controller: 'status_temp'
        }
      }
    })
	.state('tabs.status_humi', {
      url: "/status.humi",
      views: {
        'status-tab': {
          templateUrl: "templates/t.status.humi.php",
          controller: 'status_humi'
        }
      }
    })
	///////////////////////
	// Tab cong cu khac  //
	///////////////////////
    .state('tabs.setup', {
      url: "/setup",
      views: {
        'setup-tab': {
          templateUrl: "templates/t.setup.php",
          controller: 'setup'
        }
      }
    })
     .state('tabs.setup_temp', {
      url: "/setup.temp",
      views: {
        'setup-tab': {
          templateUrl: "templates/t.setup.temp.php",
          controller: 'setup_temp'
        }
      }
    })
	.state('tabs.setup_humi', {
      url: "/setup.humi",
      views: {
        'setup-tab': {
          templateUrl: "templates/t.setup.humi.php",
          controller: 'setup_humi'
        }
      }
    })

	///////////////////////
	//    Tab thiet bi    //
	///////////////////////
    .state('tabs.device', {
      url: "/device",
      views: {
        'device-tab': {
          templateUrl: "templates/t.device.php",
          controller: 'device'
        }
      }
    })
	.state('tabs.device_add', {
      url: "/device.add",
      views: {
        'device-tab': {
          templateUrl: "templates/t.device.add.php",
          controller: 'device_add'
        }
      }
    })
	.state('tabs.device_config', {
      url: "/device.config",
      views: {
        'device-tab': {
          templateUrl: "templates/t.device.config.php",
          controller: 'device_config'
        }
      }
    })
	.state('tabs.device_sms', {
      url: "/device.sms",
      views: {
        'device-tab': {
          templateUrl: "templates/t.device.sms.php",
          controller: 'device_sms'
        }
      }
    })
	.state('tabs.device_email', {
      url: "/device.email",
      views: {
        'device-tab': {
          templateUrl: "templates/t.device.email.php",
          controller: 'device_email'
        }
      }
    })
	$urlRouterProvider.otherwise("/tab/status");
})