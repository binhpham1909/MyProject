// Database instance.
var db;
// Include dependency: ngCordova
angular.module('hbiot', ['ionic'])
.controller('login', function($scope,$http) {
	$scope.login=function(){
		$http.post('process/p.login.php',{'username':$scope.username, 'password':$scope.password, 'action':'login'})
		.then(function(results) {
			console.log(JSON.stringify(results));
			if(results.data.result=="ok"){
				$scope.status=results.data.message;
				window.location.replace('index.php');
			}
			else if(results.data.result=="err"){
				$scope.status=results.data.message;
			}
		},function(err) {
			$scope.status="Error";
		});
	};
})