<?php
// We need the following information for this
//	Nameservers
//	Default domain name
//	Mailservers inc priority
//	IP address to domain
?>
<pre>
$ORIGIN <?=$origin?>.
<?=$domain?>	604800	IN	SOA	<?=$primaryns?>. registry.<?=$defaultdomain?>. (<?php date("%Y%M%d01")?> 10800 3600 604800 3600)
<?php
foreach($nameservers as $ns) {
?>
		3600	IN	NS	<?=$ns?>.
<?php
}

foreach($nameservers as $mx => $prio) {
?>
		3600	IN	MX	<?=$prio?> <?=$mx?>.
<?php
}
?>
		3600	IN	A	<?=$primaryip?>
;
$ORIGIN <?=$defaultdomain?>.
mail		3600	IN	A	<?=$primaryip?>
www		3600	IN	A	<?=$primaryip?>
</pre>
