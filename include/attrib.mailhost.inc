<?php
// attribute plugin for
// mailhost (host address which resides the mailhost)
// $Id: attrib.mailhost.inc,v 2.9 2003-08-11 19:49:52 turbo Exp $

function attribute_check($type) {
	global $options, $mailhost, $domain, $error, $LANG;
	
	// clear attribute ?
	if($options == "none")
	  $mailhost = "";
	
	// is typed in ?
	if($options == "user" and $mailhost == "") {
		$error["mailhost"] = $LANG->_('Missing');
		return false;
	}
	
	// valid
	if($options == "user" and !preg_match("/^([a-z0-9]+\.{1,1}[a-z0-9]+)+$/i",$mailhost)) {
		$error["mailhost"] = $LANG->_('Invalid');
		return false;
	}
	
	// check dns if dns option is set
	if($options  == "dns") {
		$res = getmxrr($domain, $rec, $weight);
		if(count($rec) == 0){
			$error["dns"] = $LANG->_('This domain isn\'t registred within the DNS system') . "<br>";
			return false;
		} else {
			$mailhost = $rec[0];
		}
	}
	
 	return true;
}

function attribute_init() {
	global $options, $mailhost, $oldvalue, $domain, $LANG;
	
	// control dns to set options
	$mailhost = $oldvalue;
	
	if(lc($mailhost) == lc($LANG->_('None'))) {
		$options = "none";
		$oldvalue = "";
		$mailhost = "";
	}
}

function attribute_print_form() {
	global $mailhost, $options, $domain, $user, $PHP_SELF, $attrib, $oldvalue, $error, $rootdn, $view, $LANG;
?>
  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Mail server')?></th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Options')?></td>
          <td><input type="radio" name="options" value="dns" <?php if($options=="dns"){ echo "checked"; }?>><?=$LANG->_('Standard (DNS entry)')?><input type="radio" name="options" value="user" <?php if($options=="user"){ echo "checked"; }?>><?=$LANG->_('User defined')?><input type="radio" name="options" value="none" <?php if($options=="none"){ echo "checked"; }?>><?=$LANG->_('None')?></td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Host address (FQDN)')?></td>
          <td><?php echo format_error($error["mailhost"]); ?><input type="text" name="mailhost" value="<?=$mailhost?>"></td>
        </tr>

        <tr class="subtitle">
          <td colspan="2">
            <?php echo format_error($error["dns"]); ?>
            <img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?=$LANG->_('With option DNS, the first MX record of this domain will be taken')?>
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit"   value="1">
    <input type="hidden" name="attrib"   value="<?=$attrib?>">
    <input type="hidden" name="oldvalue" value="<?=$oldvalue?>">
    <input type="hidden" name="domain"   value="<?=$domain?>">
    <input type="hidden" name="rootdn"   value="<?=$rootdn?>">
    <input type="hidden" name="user"     value="<?=$user?>">
    <input type="hidden" name="view"     value="<?=$view?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}

function attribute_save($type) {
	global $mailhost, $domain, $user, $attrib, $oldvalue, $_pql, $view, $LANG;
	
	switch($type) {
	  case "add":
		// no add operation possible with mail
		break;

	  case "modify";
		if($mailhost == $oldvalue) {
			// don't change if new value equal to old one
			$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('Mailserver')));
			attribute_forward($msg);
			break;
    	}
		
		if(pql_replace_userattribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_MAILHOST"), $mailhost)) {
			if($mailhost == "")
			  $mailhost = $LANG->_('None');

			$msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
										 array('what' => $LANG->_('mailserver'),
											   'new'  => $mailhost));
		} else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('mailserver'))) . ": " . pql_ldap_error($_pql->ldap_linkid);
		
		attribute_forward($msg);
		break;
		
	  case "fulldomain":
		$users = pql_get_user($_pql->ldap_linkid, $domain);
		
		if(is_array($users)) {
			foreach($users as $user) {
				$return[] = pql_replace_userattribute($_pql->ldap_linkid, $user, pql_get_define("PQL_GLOB_ATTR_MAILHOST"), $mailhost);
			}
			
			if(in_array(false, $return))
			  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
										   array('what' => $LANG->_('mailserver'))).": ".pql_ldap_error($_pql->ldap_linkid);
			else
			  $msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
										   array('what' => $LANG->_('mailserver'),
												 'new'  => $mailhost));
			
			attribute_forward($msg);
		}
		
	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
