<?php
// control attribute plugin for
// ldapclusterhosts (max number of bytes in message)
//
// $Id: attrib.control.ldapclusterhosts.inc,v 2.4 2004-10-18 13:39:32 turbo Exp $

// {{{ ldapclusterhosts_check(type)
function ldapclusterhosts_check($type) {
	global $error, $LANG;

	// dummy function
	return true;
}
// }}}

// {{{ ldapclusterhosts_init(host)
// init values
function ldapclusterhosts_init($host) {
	global $_pql_control;

	// fetch data from ldap server
	$_REQUEST["ldapclusterhosts"] = pql_get_attribute($_pql_control->ldap_linkid,
													  pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
													  pql_get_define("PQL_ATTR_LDAPCLUSTER_HOSTS"));

	if(is_null($_REQUEST["ldapclusterhosts"]))
	  return true;
}
// }}}


// {{{ ldapclusterhosts_print_view(host)
function ldapclusterhosts_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	ldapclusterhosts_init($host);
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left">ldapclusterhosts (<?=$LANG->_('Hosts in the mail cluster')?>)</th>
      <tr class="title">
        <td>FQDN</td>
      </tr>
<?php
	if(is_array($_REQUEST["ldapclusterhosts"])) {
		foreach($_REQUEST["ldapclusterhosts"] as $server) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td><?=pql_maybe_idna_decode($server)?></td>
      </tr>
<?php
		} // end foreach
	} // end if is_array
?>
      <tr class="subtitle">
        <td><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=ldapclusterhosts"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Add host')?></a></td>
      </tr>

      <tr class="subtitle">
        <td><img src="images/info.png" width="16" height="16" border="0"> Note: Clustering support must be compiled into qmail-ldap</td>
      </tr>
    </th>
  </table>

<?php
}
// }}}

// {{{ ldapclusterhosts_print_form(void)
function ldapclusterhosts_print_form() {
	global $error, $LANG;

	if(($_REQUEST["hostcount"] < count($_REQUEST["ldapclusterhosts"]) - 1) and count($_REQUEST["ldapclusterhosts"]) != 0)
	  $_REQUEST["hostcount"] = count($_REQUEST["ldapclusterhosts"]) - 1;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">ldapclusterhosts (<?=$LANG->_('Host to regognize as part of the mail cluster')?>)</th>
        <tr class="title">
          <td>FQDN</td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td><?php echo pql_format_error_span($error["ldapclusterhosts_0"]); ?><input type="text" name="ldapclusterhosts[0]" value="<?=$_REQUEST["ldapclusterhosts"][0]?>" size="40"></td>
        </tr>

<?php for($i = 1; $i <= $_REQUEST["hostcount"]; $i++) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td><?php echo pql_format_error_span($error["ldapclusterhosts_$i"]); ?><input type="text" name="ldapclusterhosts[<?=$i?>]" value="<?=$_REQUEST["ldapclusterhosts"][$i]?>" size="40"></td>
        </tr>

<?php } ?>
        <tr class="subtitle">
          <td><a href="<?$_SERVER["PHP_SELF"]?>?attrib=<?=$_REQUEST["attrib"]?>&hostcount=<?=($_REQUEST["hostcount"]+1)?>&mxhost=<?=$_REQUEST["mxhost"]?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('additional address'))); ?></a> (<?=$LANG->_('Please save first, changes will be lost', 4)?>)</td>
        </tr>

        <tr class="subtitle">
          <td><img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('To delete a cluster host, remove the value and save')?>.</td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="mxhost" value="<?=$_REQUEST["mxhost"]?>">
    <input type="hidden" name="view"   value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ ldapclusterhosts_save(type, host)
function ldapclusterhosts_save($type, $host)  {
	global $_pql_control, $error, $LANG;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
										 pql_get_define("PQL_ATTR_LDAPCLUSTER_HOSTS"),
										 $_REQUEST["ldapclusterhosts"]))
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('ldapclusterhosts options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('ldapclusterhosts options')))
			. ": " . pql_format_error(0);

		attribute_forward($msg);
		break;
	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}


// {{{ ldapclusterhosts_help(void)
function ldapclusterhosts_help() {
	global $LANG;
?>
    ldapclusterhosts contains a number of hostnames (FQDM) to check together
    with ~control/me when clustering is on. This file is useful if you want to
    run multiple instances of qmail-ldap on one machine.
<?php
}
// }}}

// {{{ ldapclusterhosts_help_cr(void)
function ldapclusterhosts_help_cr() {
	global $LANG;
	echo $LANG->_('Help text taken from qmail man pages');
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
