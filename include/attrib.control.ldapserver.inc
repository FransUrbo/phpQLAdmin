<?php
// control attribute plugin for
// ldapserver (server which holds ldap database)
// ldapbasedn (dn, which points to ldap database)
// ldaplogin (cn of a database user)
// ldappassword (password to bind to ldaplogin)
//
// $Id: attrib.control.ldapserver.inc,v 2.2 2002-12-17 12:24:35 turbo Exp $

function ldapserver_check($type){
	global $error, $ldapserver, $ldappassword, $ldapbasedn, $ldaplogin_use, $ldaplogin, $ldap_test;

	// missing
	if($ldapserver[0] == ""){
		$error["ldapserver_0"] = PQL_MISSING;
		$is_error = true;
	}

	// check for valid fqdn for all servers
	if(is_array($ldapserver)){
		foreach($ldapserver as $key => $server){
			if($server != "" and !check_hostaddress($server, true)){
				$error["ldapserver_$key"] = PQL_INVALID;
				$is_error = true;
			}
		}
	}
	
	// missing
	if($ldapbasedn == ""){
		$error["ldapbasedn"] = PQL_MISSING;
		$is_error = true;
	}
	
	// check for valid dn
	
	// login / password
	if($ldaplogin_use == "1"){
		// missing
		if($ldaplogin == ""){
			$error["ldaplogin"] = PQL_MISSING;
			$is_error = true;
		}
		
		// check for valid dn
		
		// missing
		if($ldappassword == ""){
			$error["ldappassword"] = PQL_MISSING;
			$is_error = true;
		}
	}

	// test connection
	if($ldap_test == "1" and $is_error == false){
		foreach($ldapserver as $server){
			if($server == ""){
				continue;
			}
			
			$linkid = @ldap_connect($server);
			
			if($linkid){
				if($ldaplogin_use == "1"){
					if(!@ldap_bind($linkid, $ldaplogin, $ldappassword)){
						$error["ldap_test"] .= "Failed running a test connection to $server (" . ldap_error($linkid) . ")<br>";
						$is_error = true;
					}
				} else {
					if(!@ldap_bind($linkid)){
						$error["ldap_test"] .= "Failed running a test connection to $server (" . ldap_error($linkid) . ")<br>";
						$is_error = true;
					}
				}
			} else {
				// connection failed
				$error["ldap_test"] .= "Failed running a test connection to $server (server not responding or invalid name)<br>";
				$is_error = true;
			}
			
			// search for base dn
			if($is_error == false){
				// how to check basedn ???
			}
			
			@ldap_unbind($linkid);
			@ldap_close($linkid);
		}
	}

	if($is_error == false){
		return true;
 	}
}

function ldapserver_init($host) {
	// init values
	global $_pql_control, $srvcount, $ldapserver, $ldapbasedn, $ldaplogin_use, $ldaplogin, $ldappassword;
	
	// fetch data from ldap server
	$attribs = array("ldapserver", "ldapbasedn", "ldaplogin", "ldappassword");
	
	foreach($attribs as $attrib){
		$value = pql_control_get_attribute($_pql_control->ldap_linkid,
										   "cn=" . $host . "," . $USER_SEARCH_DN_CTR,
										   $attrib);
		
		if(!is_null($value)){
			$$attrib = $value[0];
		}
	}
	
	// init checkbox
	if($ldaplogin != ""){
		$ldaplogin_use = 1;
	}
	
	// map different hostnames sperated by space to an array
	$ldapserver = split(" ", $ldapserver);
	if($srvcount == ""){
		$srvcount = count($ldapserver) - 1;
	}
}

function ldapserver_print_view($host) {
	global $_pql_control, $ldapserver, $ldapbasedn, $ldaplogin_use, $ldaplogin, $ldappassword;

	// init data
	ldapserver_init($host);

	// don't display passwords
	if($ldappassword != ""){
		$ldappassword = "set, but now shown";
 	}

	// set empty field to "not set"
	$attribs = array("ldapserver", "ldapbasedn", "ldaplogin", "ldappassword");

	foreach($attribs as $attrib){
		if($$attrib == ""){
			$$attrib = "not set";
		}
	}
	
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">server connection options</th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Host(s)</td>
        <td><?php echo join(", ", $ldapserver); ?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Base DN</td>
        <td><?php echo $ldapbasedn; ?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">Login</td>
        <td><?php echo $ldaplogin; ?></td>
      </tr>

      <tr class="<?php table_bgcolor(); ?>">
        <td class="title"><?php echo PQL_LDAP_USERPASSWORD_TITLE; ?></td>
        <td><?php echo $ldappassword; ?></td>
      </tr>

<?php if($ldapserver == "not set" or $ldapbasedn == "not set"){ ?>
      <tr class="subtitle">
        <td colspan="2"><span class="error">You have to set both, host and base dn to make qmail-ldap work.</td>
      </tr>

<?php } ?>
      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?attrib=ldapserver"><img src="images/edit.png" width="12" height="12" border="0"> change options</a></td>
      </tr>
    </th>
  </table>

<?php
}

function ldapserver_print_form(){
	global $attrib, $srvcount, $ldapserver, $ldapbasedn, $ldaplogin_use, $ldaplogin, $ldap_test, $PHP_SELF, $error;
	
?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">LDAP server connection option</th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Host(s)</td>
          <td><?php echo format_error($error["ldapserver_0"]); ?><input type="text" name="ldapserver[0]" value="<?php echo $ldapserver[0]; ?>">
		    <br>
<?php for($i = 1; $i <= $srvcount; $i++){ ?>
			<?php echo format_error($error["ldapserver_$i"]); ?><input type="text" name="ldapserver[<?php echo $i; ?>]" value="<?php echo $ldapserver[$i]; ?>"><br>
<?php } ?>
            <br>
            <a href="<?php echo $PHP_SELF; ?>?attrib=<?php echo $attrib; ?>&srvcount=<?php echo ($srvcount + 1); ?>">add more servers</a>
          </td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Base DN</td>
          <td><?php echo format_error($error["ldapbasedn"]); ?><input type="text" name="ldapbasedn" value="<?php echo $ldapbasedn; ?>"></td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td colspan="2"><input type="checkbox" name="ldaplogin_use" value="1" <?php if($ldaplogin_use == "1"){ echo "checked"; } ?>> use login and password</td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Login (valid dn)</td>
          <td><?php echo format_error($error["ldaplogin"]); ?><input type="text" name="ldaplogin" value="<?php echo $ldaplogin; ?>"></td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?php echo PQL_LDAP_USERPASSWORD_TITLE; ?></td>
          <td><?php echo format_error($error["ldappassword"]); ?><input type="password" name="ldappassword"></td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td colspan="2"><input type="checkbox" name="ldap_test" value="1" <?php if($ldap_test == "1"){ echo "checked"; } //" ?>>
			test connection after submitting form (it will be tested for each server seperatly)
          </td>
        </tr>

<?php if($error["ldap_test"] != ""){ ?>
        <tr class="<?php table_bgcolor(); ?>">
          <td colspan="2"><?php echo format_error($error["ldap_test"]); ?></td>
        </tr>
<?php } ?>
      </th>
    </table>

    <input type="hidden" name="srvcount" value="<?php echo $srvcount; ?>">
    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?php echo $attrib; ?>">
    <input type="submit" value="<?php echo PQL_SAVE; ?>">
  </form>

<?php
}

function ldapserver_save($type, $host) {
	global $_pql_control, $attrib, $ldapserver, $ldapbasedn, $ldaplogin_use, $ldaplogin,$ldappassword, $ldap_test, $error;
	
	// recontrol values
	if(!ldapserver_check($type)){
 		return false;
	}
	
	// map ldapserver array to a single string
	$ldapserver = trim(join(" ", $ldapserver));
	
	// remove all double, triple or whatever spaces, replace with single one
	$ldapserver = preg_replace("/\s+/", " ", $ldapserver);
	
	switch($type){
		case "modify";
			// save entries
			$return[] = pql_control_replace_attribute($_pql_control->ldap_linkid,
													  "cn=" . $host . "," . $USER_SEARCH_DN_CTR,
													  "ldapserver", $ldapserver);
			$return[] = pql_control_replace_attribute($_pql_control->ldap_linkid,
													  "cn=" . $host . "," . $USER_SEARCH_DN_CTR,
													  "ldapbasedn", $ldapbasedn);

			if($ldaplogin_use == "1"){
				$return[] = pql_control_replace_attribute($_pql_control->ldap_linkid,
														  "cn=" . $host . "," . $USER_SEARCH_DN_CTR,
														  "ldaplogin", $ldaplogin);
				$return[] = pql_control_replace_attribute($_pql_control->ldap_linkid,
														  "cn=" . $host . "," . $USER_SEARCH_DN_CTR,
														  "ldappassword", $ldappassword);
			} else {
				// reset login and password values
				$return[] = pql_control_replace_attribute($_pql_control->ldap_linkid,
														  "cn=" . $host . "," . $USER_SEARCH_DN_CTR,
														  "ldaplogin", "");
				$return[] = pql_control_replace_attribute($_pql_control->ldap_linkid,
														  "cn=" . $host . "," . $USER_SEARCH_DN_CTR,
														  "ldappassword", "");
    	}

		if(in_array(false, $return)){
			$msg = "Error saving ldapserver options: " . pql_ldap_error($_pql_control->ldap_linkid);
		} else {
			$msg = "Successfully saved ldapserver options";
    	}

		attribute_forward($msg);
		break;
	  default:
		die("unknown save type $type in " . __FILE__ . ", function save()");
	}
}

function ldapserver_help(){
	// help text taken from
	// life with qmail-ldap (http://www.lifewithqmail.org/ldap/)
	?>
	<b>Host</b><br>
	Your ldap server's hostname. If you want more than one ldap-server for redundancy,
	use the "add more servers" link to add another server. To clear additional server simply
	remove the value and save.
	<br><br>
	<b>Base DN</b><br>
  The BaseDN for ldap searches. See OpenLDAP's documentation for more information about BaseDN. Required.
  Example: "o=yourcorp, c=ch"
	<br><br>
  <b>Login</b><br>
	If you need to authentificate against your ldap server to retrieve the user information, this is the username to do so.
	Note: this is a ldap dn, not a unix username.
	<br><br>
  <b><?php echo PQL_LDAP_USERPASSWORD_TITLE; ?></b><br>
	The password for the user defined in ldaplogin if needed. Cleartext.
	<?php
}

function ldapserver_help_cr(){
?>
	help text taken from life with qmail-ldap
<?php
}
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
