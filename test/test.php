<!DOCTYPE HTML>
<html>
	<head>
		<title>UniversalBot - test</title>
	</head>
	<body>
		<?php
			$dataBaseServer = "";
			$userName = "";
			$userPassword = "";
			$dataBase = ""
			if ($_SERVER['REQUEST_METHOD'] === 'POST') 
			{
				$isPost = 'yes';
			}
			else
			{
				$isPost = 'no';
			}
			$con = mysqli_connect($dataBaseServer, $userName, $userPassword, $dataBase);
			mysqli_query($con, "insert into connections(proxy, date, postData, getData, isPost) values('".$_SERVER['REMOTE_ADDR']."',now(),'".$_POST['postData']."','".$_GET['getData']."','".$isPost."')");
			mysqli_close($con);
			echo 'Post: '.$_POST['postData'].'<br/>';
			echo 'GET: '.$_GET['getData'].'<br/>';
			echo 'REMOTE_ADDR: '.$_SERVER['REMOTE_ADDR'].'<br/>';
		?>
	</body>
</html>