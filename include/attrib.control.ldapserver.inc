<?php
// control attribute plugin for
// ldapserver (server which holds ldap database)
// ldapbasedn (dn, which points to ldap database)
// ldaplogin (cn of a database user)
// ldappassword (password to bind to ldaplogin)
//
// $Id: attrib.control.ldapserver.inc,v 2.12 2004-02-14 14:25:36 turbo Exp $

function ldapserver_check($type) {
	global $error, $ldapserver, $ldappassword, $ldapbasedn, $ldaplogin_use, $ldaplogin, $ldap_test, $view, $LANG;

	// missing
	if($ldapserver[0] == "") {
		$error["ldapserver_0"] = $LANG->_('Missing');
		$is_error = true;
	}

	// check for valid fqdn for all servers
	if(is_array($ldapserver)) {
		foreach($ldapserver as $key => $server) {
			if($server != "" and !pql_check_hostaddress($server, true)) {
				$error["ldapserver_$key"] = $LANG->_('Invalid');
				$is_error = true;
			}
		}
	}
	
	// missing
	if($ldapbasedn == "") {
		$error["ldapbasedn"] = $LANG->_('Missing');
		$is_error = true;
	}
	
	// check for valid dn
	
	// login / password
	if($ldaplogin_use == "1") {
		// missing
		if($ldaplogin == "") {
			$error["ldaplogin"] = $LANG->_('Missing');
			$is_error = true;
		}
		
		// check for valid dn
		
		// missing
		if($ldappassword == "") {
			$error["ldappassword"] = $LANG->_('Missing');
			$is_error = true;
		}
	}

	// test connection
	if($ldap_test == "1" and $is_error == false) {
		foreach($ldapserver as $server) {
			if($server == "")
			  continue;
			
			$linkid = @ldap_connect($server);
			
			if($linkid) {
				if($ldaplogin_use == "1") {
					if(!@ldap_bind($linkid, $ldaplogin, $ldappassword)) {
						$error["ldap_test"] .= pql_complete_constant($LANG->_('Failed running a test connection to %server%'),
																	 array('server' => $server)) . ": " . ldap_error($linkid) . "<br>";
						$is_error = true;
					}
				} else {
					if(!@ldap_bind($linkid)) {
						$error["ldap_test"] .= pql_complete_constant($LANG->_('Failed running a test connection to %server%'),
																	 array('server' => $server)) . ": " . ldap_error($linkid) . "<br>";
						$is_error = true;
					}
				}
			} else {
				// connection failed
				$error["ldap_test"] .= pql_complete_constant($LANG->_('Failed running a test connection to %server%'),
															 array('server' => $server))
				  . '(' . $LANG->_('server not responding or invalid name') . ')<br>';
				$is_error = true;
			}
			
			// search for base dn
			if($is_error == false)
			  // TODO: How to check basedn???
			
			@ldap_unbind($linkid);
			@ldap_close($linkid);
		}
	}

	if($is_error == false)
	  return true;
}

// init values
function ldapserver_init($host) {
	global $_pql_control, $srvcount, $ldapserver, $ldapbasedn, $ldaplogin_use, $ldaplogin, $ldappassword, $view;
	
	// fetch data from ldap server
	$attribs = array(pql_get_define("PQL_GLOB_ATTR_LDAPSERVER"),
					 pql_get_define("PQL_GLOB_ATTR_LDAPBASEDN"),
					 pql_get_define("PQL_GLOB_ATTR_LDAPLOGIN"),
					 pql_get_define("PQL_GLOB_ATTR_LDAPPASSWORD"));
	
	foreach($attribs as $attrib) {
		$value = pql_control_get_attribute($_pql_control->ldap_linkid,
										   pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
										   $attrib);
		
		if(!is_null($value))
		  $$attrib = $value[0];
	}
	
	// init checkbox
	if($ldaplogin != "")
	  $ldaplogin_use = 1;
	
	// map different hostnames sperated by space to an array
	$ldapserver = split(" ", $ldapserver);
	if($srvcount == "")
	  $srvcount = count($ldapserver) - 1;
}

function ldapserver_print_view($host) {
	global $_pql_control, $ldapserver, $ldapbasedn, $ldaplogin_use, $ldaplogin, $ldappassword, $view, $LANG;

	// init data
	ldapserver_init($host);

	// don't display passwords
	if($ldappassword != "")
	  $ldappassword = $LANG->_('Set, but now shown');

	// set empty field to "not set"
	$attribs = array("ldapserver", "ldapbasedn", "ldaplogin", "ldappassword");
	foreach($attribs as $attrib) {
		if($$attrib == "")
		  $$attrib = $LANG->_('Not set');
	}
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Server connection options')?></th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Host(s)')?></td>
        <td><?php echo join(", ", $ldapserver); ?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Base DN')?></td>
        <td><?=$ldapbasedn?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Login')?></td>
        <td><?=$ldaplogin?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Password')?></td>
        <td><?=$ldappassword?></td>
      </tr>

<?php if($ldapserver == $LANG->_('Not set') or $ldapbasedn == $LANG->_('Not set')) { ?>
      <tr class="subtitle">
        <td colspan="2"><span class="error"><?=$LANG->_('You have to set both, host and base dn to make qmail-ldap work')?>.</td>
      </tr>

<?php } ?>
      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=ldapserver"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>
<?php
}

function ldapserver_print_form() {
	global $attrib, $srvcount, $ldapserver, $ldapbasedn, $ldaplogin_use, $ldaplogin, $ldap_test, $error, $LANG;
	global $mxhost, $view;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('LDAP server connection option')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Host(s)')?></td>
          <td><?php echo pql_format_error_span($error["ldapserver_0"]); ?><input type="text" name="ldapserver[0]" value="<?=$ldapserver[0]?>">
		    <br>
<?php for($i = 1; $i <= $srvcount; $i++){ ?>
			<?php echo pql_format_error_span($error["ldapserver_$i"]); ?><input type="text" name="ldapserver[<?=$i?>]" value="<?=$ldapserver[$i]?>"><br>
<?php } ?>
            <br>
            <a href="<?=$_SERVER["PHP_SELF"]?>?attrib=<?=$attrib?>&srvcount=<?php echo ($srvcount + 1); ?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('more servers'))); ?></a>
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Base DN')?></td>
          <td><?php echo pql_format_error_span($error["ldapbasedn"]); ?><input type="text" name="ldapbasedn" value="<?=$ldapbasedn?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td colspan="2"><input type="checkbox" name="ldaplogin_use" value="1" <?php if($ldaplogin_use == "1"){ echo "checked"; } ?>> <?=$LANG->_('Use login and password')?></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Login (valid dn)')?></td>
          <td><?php echo pql_format_error_span($error["ldaplogin"]); ?><input type="text" name="ldaplogin" value="<?=$ldaplogin?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Password')?></td>
          <td><?php echo pql_format_error_span($error["ldappassword"]); ?><input type="password" name="ldappassword"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td colspan="2"><input type="checkbox" name="ldap_test" value="1" <?php if($ldap_test == "1"){ echo "checked"; } //" ?>>
			<?=$LANG->_('Test connection after submitting form (it will be tested for each server seperatly)')?>
          </td>
        </tr>

<?php if($error["ldap_test"] != ""){ ?>
        <tr class="<?php pql_format_table(); ?>">
          <td colspan="2"><?php echo pql_format_error_span($error["ldap_test"]); ?></td>
        </tr>
<?php } ?>
      </th>
    </table>

    <input type="hidden" name="srvcount" value="<?=$srvcount?>">
    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?=$attrib?>">
    <input type="hidden" name="mxhost" value="<?=$mxhost?>">
    <input type="hidden" name="view" value="<?=$view?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>

<?php
}

function ldapserver_save($type, $host) {
	global $_pql_control, $attrib, $ldapserver, $ldapbasedn, $ldaplogin_use, $ldaplogin,$ldappassword, $ldap_test, $error, $LANG;
	global $view;
	
	// map ldapserver array to a single string
	$ldapserver = trim(join(" ", $ldapserver));
	
	// remove all double, triple or whatever spaces, replace with single one
	$ldapserver = preg_replace("/\s+/", " ", $ldapserver);
	
	switch($type) {
		case "modify";
		case "add";
			// save entries
		$attribs = array(pql_get_define("PQL_GLOB_ATTR_LDAPSERVER"),
						 pql_get_define("PQL_GLOB_ATTR_LDAPBASEDN"));
		foreach($attribs as $attrib)
		  $return[] = pql_control_replace_attribute($_pql_control->ldap_linkid,
													pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
													$attrib, $$attrib);
		
		$attribs = array(pql_get_define("PQL_GLOB_ATTR_LDAPLOGIN"),
						 pql_get_define("PQL_GLOB_ATTR_LDAPPASSWORD"));
		if($ldaplogin_use == "1"){
			foreach($attribs as $attrib)
			  $return[] = pql_control_replace_attribute($_pql_control->ldap_linkid,
														pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
														$attrib, $$attrib);
		} else {
			foreach($attribs as $attrib)
			  // reset login and password values
			  $return[] = pql_control_replace_attribute($_pql_control->ldap_linkid,
														pql_get_define("PQL_GLOB_ATTR_CN") . "=" . $host . "," . $_SESSION["USER_SEARCH_DN_CTR"],
														$attrib, "");
    	}

		if(in_array(false, $return))
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('ldapserver options')))
			. ": " . pql_format_error(0);
		else
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('ldapserver options')));

		attribute_forward($msg);
		break;

	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}

function ldapserver_help() {
	global $LANG;
?>
	<b><?=$LANG->_('Host')?></b><br>
	Your ldap server's hostname. If you want more than one ldap-server for redundancy,
	use the "add more servers" link to add another server. To clear additional server simply
	remove the value and save.
	<br><br>
	<b><?=$LANG->_('Base DN')?></b><br>
  The BaseDN for ldap searches. See OpenLDAP's documentation for more information about BaseDN. Required.
  Example: "o=yourcorp, c=ch"
	<br><br>
  <b><?=$LANG->_('Login')?></b><br>
	If you need to authentificate against your ldap server to retrieve the user information, this is the username to do so.
	Note: this is a ldap dn, not a unix username.
	<br><br>
  <b><?=$LANG->_('Password')?></b><br>
	The password for the user defined in ldaplogin if needed. Cleartext.
	<?php
}

function ldapserver_help_cr() {
	global $LANG;
	echo $LANG->_('Help text taken from life with qmail-ldap');
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
