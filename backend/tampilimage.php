<?php
require_once('koneksi.php');
$sql="SELECT * FROM tbinfus";
$r=mysqli_query($con,$sql);
$result=array();
$url="http://192.168.137.102/tampil.php?id=";
while($row=mysqli_fetch_array($r))
	{
	array_push($result,array('url'=>$url.$row['id'],'id'=>$row['id']));
	}
echo json_encode(array("result"=>$result));
mysqli_close($con);
?>
