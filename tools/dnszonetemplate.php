<html>
<body>
<?php
// $Id: dnszonetemplate.php,v 1.3 2003-01-13 10:47:38 turbo Exp $
// You can either use this template as is, or write you own,
// with predefined values (staticly set)...
//
// This template looks in the DNS system for default values,
// but this will naturaly fail if it isn't registered. And it'
// no point in CREATING a template if it ISN'T registered, so...
//
$dnsparts = split('\.', $defaultdomain);
for($i=0; $dnsparts[$i]; $i++) {
    $origin = $dnsparts[$i];
}

// Just incase we haven't defined the TLD for this domain
// we must have the top origin (dot).
if($origin == $defaultdomain) {
    $origin = '';
}

// Create a serial number for zone
$date = date("Ymd01");

$res = getmxrr($defaultdomain, $rec, $weight);
if(count($weight)) {
    asort($weight);
} else {
    // Domain not registered in DNS, make up values
    $rec = array('<b>REPLACE_WITH_IP_TO_MAILSERVER</b>');
    $weight = array(10);
}

// Get Primary IP address
if(checkdnsrr($defaultdomain.'.', 'NS')) {
    $primaryip = gethostbyname($defaultdomain.'.');
}
if(!$primaryip) {
    $primaryip = '<b>REPLACE_WITH_IP_TO_DOMAIN</b>';
}

$nameservers = array('ns1.'.$defaultdomain);

?>
<pre>
$ORIGIN <?=$origin?>.
<?=$domain?>	604800	IN	SOA	<?=$nameservers[0]?>. registry.<?=$defaultdomain?>. (
					<?=$date?>  ; Serial number
					     10800  ; Refresh
					      3600  ; Retry
					    604800  ; Expire
					      3600) ; Negative Cache TTL
; Name-/Mailservers
<?php
foreach($nameservers as $ns) {
?>
		3600	IN	NS	<?=$ns?>.
<?php
}
?>
<?php
foreach($weight as $key => $prio) {
?>
		3600	IN	MX	<?=$prio?> <?=$rec[$key]?>.
<?php
}
?>
; Host records
		3600	IN	A	<?=$primaryip?>

$ORIGIN <?=$defaultdomain?>.
ns1		3600	IN	A	<?=$primaryip?>

mail		3600	IN	A	<?=$primaryip?>

www		3600	IN	A	<?=$primaryip?>
</pre>
</body>
</html>
