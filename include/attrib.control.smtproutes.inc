<?php
// control attribute plugin for
// smtproutes (max number of bytes in message)
//
// $Id: attrib.control.smtproutes.inc,v 2.1 2002-12-17 06:21:05 turbo Exp $

function smtproutes_check($type) {
	global $error, $domain, $relay, $port;

	// check all domains
	if(is_array($domain)) {
		foreach($domain as $key => $dom) {
			if(preg_match("/[^0-9a-z-\.]+/i",$dom)) {
				$error["domain_$key"] = PQL_INVALID;
				$is_error = true;
			}
		}
	}

	// relay hosts
	if(is_array($relay)) {
		foreach($relay as $key => $rel) {
			if($rel != "" and !check_hostaddress($rel)) {
				$error["relay_$key"] = PQL_INVALID;
				$is_error = true;
			}
		}
	}

	// ports
	if(is_array($port)){
		foreach($port as $key => $prt){
			if(preg_match("/[^0-9]+/i", $prt)){
				$error["port_$key"] = PQL_INVALID;
				$is_error = true;
			}
		}
	}
	
	if($is_error){
		return false;
	}
	
	return true;
}

function smtproutes_init($host) {
	// init values
	global $_pql_control, $smtproutes, $domain, $relay, $port;

	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid,
									   "cn=" . $host . "," . PQL_LDAP_CONTROL_BASEDN,
									   "smtproutes");

	if(is_null($value)){
		return true;
	}

	foreach($value as $route){
		$smtproutes[] = split(":", $route);
	}

	foreach($smtproutes as $route){
		$domain[] = $route[0];
		$relay[] = $route[1];
		$port[] = $route[2];
	}
}

function smtproutes_print_view($host) {
	global $_pql_control, $smtproutes;

	// init data
	smtproutes_init($host);

	if(is_array($smtproutes)){
		foreach($smtproutes as $route){
			// domain
			if($route[0] == ""){
				$route[0] = "&lt; all &gt;";
			}
			
			// relay host
			if($route[1] == ""){
				$route[1] = "MX-record";
			}
			
			// port
			if($route[2] == ""){
				$route[2] = "25";
			}
			
			$routes[] = $route;
		}
	}
	
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">smtproutes (Defined SMTP routes)</th>
      <tr class="title">
        <td>Source domain</td>
        <td>&nbsp;</td>
        <td>Relay host</td>
        <td>&nbsp;</td>
        <td>Port</td>
      </tr>
<?php
	if(!is_array($routes)){
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td colspan="5">no routes defined</td>
      </tr>
<?php
	} else {
		foreach($routes as $route){
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td><?php echo $route[0]; ?></td>
        <td><b>=&gt;</b></td>
        <td><?php echo $route[1]; ?></td>
        <td><b>:</b></td>
        <td><?php echo $route[2]; ?></td>
      </tr>
<?php
		} // end foreach
	} // end if is_array
?>
      <tr class="subtitle">
        <td colspan="5"><a href="control_edit_attribute.php?attrib=smtproutes"><img src="images/edit.png" width="12" height="12" border="0"> change options</a></td>
      </tr>
    </th>
  </table>

<?php
}

function smtproutes_print_form() {
	global $attrib, $PHP_SELF, $error, $domain, $relay, $port, $routecount;

	if(($routecount < count($domain) - 1) and count($domain) != 0){
		$routecount = count($domain) - 1;
	}

?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">smtproutes (SMTP routes)</th>
        <tr class="title">
          <td>Source domain</td>
          <td>Relay host</td>
          <td>Port</td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td><?php echo format_error($error["domain_0"]); ?><input type="text" name="domain[0]" value="<?php echo $domain[0]; ?>">&nbsp;&nbsp;<b>=></b></td>
          <td><?php echo format_error($error["relay_0"]); ?><input type="text" name="relay[0]" value="<?php echo $relay[0]; ?>">&nbsp;&nbsp;<b>:</b></td>
          <td><?php echo format_error($error["port_0"]); ?><input type="text" size="4" maxlength="4" name="port[0]" value="<?php echo $port[0]; ?>"></td>
        </tr>

<?php for($i = 1; $i <= $routecount; $i++){ ?>
        <tr class="<?php table_bgcolor(); ?>">
          <td><?php echo format_error($error["domain_$i"]); ?><input type="text" name="domain[<?php echo $i; ?>]" value="<?php echo $domain[$i]; ?>">&nbsp;&nbsp;<b>=></b></td>
          <td><?php echo format_error($error["relay_$i"]); ?><input type="text" name="relay[<?php echo $i; ?>]" value="<?php echo $relay[$i]; ?>">&nbsp;&nbsp;<b>:</b></td>
          <td><?php echo format_error($error["port_$i"]); ?><input type="text" size="4" maxlength="4" name="port[<?php echo $i; ?>]" value="<?php echo $port[$i]; ?>"></td>
        </tr>

<?php } ?>
        <tr class="subtitle">
          <td colspan="3"><a href="<?php echo $PHP_SELF; ?>?attrib=<?php echo $attrib; ?>&routecount=<?php echo ($routecount + 1); ?>">add additional route</a> (please save first, changes will be lost)</td>
        </tr>

        <tr class="subtitle">
          <td colspan="3"><img src="images/info.png" width="16" height="16" border="0"> If the source domain is empty, the route affects to all hosts. If relay host is empty, qmail will take the MX record of the domain to relay. Use port if the smtp port of the relay host is not 25. Leave domain and relay empty to delete a route.</td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?php echo $attrib; ?>">
    <input type="submit" value="<?php echo PQL_SAVE; ?>">
  </form>

<?php
}

function smtproutes_save($type, $host) {
	global $_pql_control, $attrib, $smtproutes, $error, $domain, $relay, $port;

	// recontrol values
	if(!smtproutes_check($type)){
 		return false;
	}

	switch($type){
	  case "modify";
		// save entries - combine attributes
		for($i = 0; $i <= count($domain) - 1; $i++){
			// if empty, delete route
			if($domain[$i] == "" and $relay[$i] == ""){
				continue;
     		}
			
			// if port or relay is not defined, leave out
			if($port[$i] == "" or $relay[$i] == ""){
				$tmp = array($domain[$i], $relay[$i]);
     		} else {
				$tmp = array($domain[$i], $relay[$i], $port[$i]);
			}
			
			$route = join(":", $tmp);
			$smtproutes[] = $route;
		}

		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 "cn=" . $host . "," . PQL_LDAP_CONTROL_BASEDN,
										 "smtproutes", $smtproutes)){
			$msg = "Successfully saved smtproutes options";
		} else {
			$msg = "Failed saving smtproutes: " . pql_ldap_error($_pql_control->ldap_linkid);
    	}

		attribute_forward($msg);
		break;
   	  default:
		die("unknown save type $type in " . __FILE__ . ", function save()");
	}
}

function smtproutes_help(){
?>
	Artificial SMTP routes.  Each route has the form
	domain:relay, without any extra spaces.  If domain
	matches host, qmail-remote will connect to relay, as if
	host had relay as its only MX.  (It will also avoid
	doing any CNAME lookups on recip.)  host may include a
	colon and a port number to use instead of the normal
	SMTP port, 25:
	<br><br>
			inside.af.mil:firewall.af.mil:26
	<br><br>
	relay may be empty; this tells qmail-remote to look up
	MX records as usual.  smtproutes may include wildcards:
	<br><br>
			.af.mil:<br>
			:heaven.af.mil
	<br><br>
	Here any address ending with .af.mil (but not af.mil
	itself) is routed by its MX records; any other address
	is artificially routed to heaven.af.mil.
	<br><br>
	The qmail system does not protect you if you create an
	artificial mail loop between machines.  However, you
	are always safe using smtproutes if you do not accept
	mail from the network.
<?php
}

function smtproutes_help_cr(){
?>
	help text taken from qmail man pages
<?php
}
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
