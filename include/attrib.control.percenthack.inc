<?php
// control attribute plugin for
// percenthack (allowed domains for %-relaying)
//
// $Id: attrib.control.percenthack.inc,v 2.2 2002-12-17 12:24:35 turbo Exp $

function percenthack_check($type) {
	global $error, $percenthack;
	
	// check each field
	if(is_array($percenthack)){
		foreach($percenthack as $key => $server){
			if($server == ""){
				continue;
			}
			
			// it is a valid domain name
			if(!check_hostaddress($server)){
				$is_error = true;
				$error["percenthack_$key"] = PQL_INVALID;
			}
			
		} // end foreach
	} // end if is_array...

	if($is_error){
		return false;
	}
	
	return true;
}

function percenthack_init($host) {
	// init values
	global $_pql_control, $percenthack;
	
	// fetch data from ldap server
	$percenthack = pql_control_get_attribute($_pql_control->ldap_linkid,
											 "cn=" . $host . "," . $USER_SEARCH_DN_CTR,
											 "percenthack");
	
	if(is_null($percenthack)){
		return true;
	}
}

function percenthack_print_view($host) {
	global $_pql_control, $percenthack;
	
	// init data
	percenthack_init($host);

?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left">percenthack (percent relaying domains)</th>
      <tr class="title">
        <td>Domain</td>
      </tr>
 <?php
	if(!is_array($percenthack)){
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td>no domains defined</td>
      </tr>
<?php
	} else {
		foreach($percenthack as $domain){
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td><?php echo $domain; ?></td>
      </tr>
<?php
		} // end foreach
	} // end if is_array
?>
      <tr class="subtitle">
        <td><a href="control_edit_attribute.php?attrib=percenthack"><img src="images/edit.png" width="12" height="12" border="0"> change options</a></td>
      </tr>
    </th>
  </table>

<?php
}

function percenthack_print_form() {
	global $attrib, $PHP_SELF, $error, $percenthack, $percentcount;

	if(($percentcount < count($percenthack) - 1) and count($percenthack) != 0){
		$percentcount = count($percenthack) - 1;
	}

?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">percenthack (percent relaying domains)</th>
        <tr class="title">
          <td>Domains</td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td><?php echo format_error($error["percenthack_0"]); ?><input type="text" name="percenthack[0]" value="<?php echo $percenthack[0]; ?>"></td>
        </tr>

<?php for($i = 1; $i <= $percentcount; $i++){ ?>
        <tr class="<?php table_bgcolor(); ?>">
          <td><?php echo format_error($error["percenthack_$i"]); ?><input type="text" name="percenthack[<?php echo $i; ?>]" value="<?php echo $percenthack[$i]; ?>"></td>
        </tr>

<?php } ?>
        <tr class="subtitle">
          <td><a href="<?php echo $PHP_SELF; ?>?attrib=<?php echo $attrib; ?>&percentcount=<?php echo ($percentcount + 1); ?>">add additional domain</a> (please save first, changes will be lost)</td>
        </tr>

        <tr class="subtitle">
          <td><img src="images/info.png" width="16" height="16" border="0"> To delete a domain remove the value and save.</td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?php echo $attrib; ?>">
    <input type="submit" value="<?php echo PQL_SAVE; ?>">
  </form>

<?php
}

function percenthack_save($type, $host) {
	global $_pql_control, $attrib, $percenthack, $error;

	// recontrol values
	if(!percenthack_check($type)){
 		return false;
	}

	switch($type){
	  case "modify";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 "cn=" . $host . "," . $USER_SEARCH_DN_CTR,
										 "percenthack", $percenthack)) {
			$msg = "Successfully saved percenthack options";
		} else {
			$msg = "Failed saving percenthack: " . pql_ldap_error($_pql_control->ldap_linkid);
    	}

		attribute_forward($msg);
		break;
	  default:
		die("unknown save type $type in " . __FILE__ . ", function save()");
	}
}

function percenthack_help() {
?>
	List of domain names where the percent hack is applied.
	If domain is listed in percenthack, any address of the
	form user%fqdn@domain is rewritten as user@fqdn.  user
	may contain %, so the percent hack may be applied
	repeatedly.  qmail-send handles percenthack before
	locals.
<?php
}

function percenthack_help_cr(){
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
