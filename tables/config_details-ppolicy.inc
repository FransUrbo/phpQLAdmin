<?php
// Plugin for the ppolicy overlay in OpenLDAP >v2.3
// $Id: config_details-ppolicy.inc,v 1.4 2008-03-19 13:24:32 turbo Exp $
require($_SESSION["path"]."/include/pql_ppolicy.inc");
$ppolicies = pql_get_ppolicies();

// {{{ Simplicity - with this we can have ONE single foreach() loop
// to output the whole view!
// TODO: Do this elsewhere to :)
$table_columns = array(pql_get_define("PQL_ATTR_PPOLICY_NAME")					=> $LANG->_('Password Policy Name'),
					   pql_get_define("PQL_ATTR_PPOLICY_DESC")					=> $LANG->_('Password Policy Description'),
					   pql_get_define("PQL_ATTR_PPOLICY_ATTR")					=> $LANG->_('Password attribute'),
					   pql_get_define("PQL_ATTR_PPOLICY_MINAGE")				=> $LANG->_('Minimum age'),
					   pql_get_define("PQL_ATTR_PPOLICY_MAXAGE")				=> $LANG->_('Maximum age'),
					   pql_get_define("PQL_ATTR_PPOLICY_INHIST")				=> $LANG->_('Numer of old passwords in history'),
					   pql_get_define("PQL_ATTR_PPOLICY_CHKQUAL")				=> $LANG->_('Password control/verification type'),
					   pql_get_define("PQL_ATTR_PPOLICY_MINLEN")				=> $LANG->_('Minimum password length'),
					   pql_get_define("PQL_ATTR_PPOLICY_EXPIRE_WARNING")		=> $LANG->_('Password expiration warning'),
					   pql_get_define("PQL_ATTR_PPOLICY_GRACEAUTHLIMIT")		=> $LANG->_('Number of times to allow expired password'),
					   pql_get_define("PQL_ATTR_PPOLICY_LOCKOUT")				=> $LANG->_('Lock account after failed attempt'),
					   pql_get_define("PQL_ATTR_PPOLICY_LOCKOUT_DURATION")		=> $LANG->_('Time between failed attempt and new attempt'),
					   pql_get_define("PQL_ATTR_PPOLICY_MAXFAILURE")			=> $LANG->_('Maximum failed attempts before lock'),
					   pql_get_define("PQL_ATTR_PPOLICY_FAILURE_COUNT_INTERVAL")=> $LANG->_('Time for old failed attempts to be forgotten'),
					   pql_get_define("PQL_ATTR_PPOLICY_MUST_CHANGE")			=> $LANG->_('Force password change'),
					   pql_get_define("PQL_ATTR_PPOLICY_ALLOW_USER_CHANGE")		=> $LANG->_('Allow user to change password'),
					   pql_get_define("PQL_ATTR_PPOLICY_SAFE_MODIFY")			=> $LANG->_('Force the use of old password when changing'),
					   pql_get_define("PQL_ATTR_PPOLICY_CHECK_MODULE")			=> $LANG->_('Additional password check module'));
// }}}
?>
  <br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?php echo $LANG->_('Global Password Policies')?>
<?php for($i=0; $ppolicies[$i]; $i++) {
		foreach($table_columns as $attrib => $text) {
		  $link = 'config_edit_attribute.php?rootdn='.urlencode($ppolicies[$i]["dn"]).'&attrib='.$attrib;
		  $class=pql_format_table(0);
?>
      <tr>
        <td class="title"><?php echo $text?></td>
<?php	  if($attrib == sprintf("%s", pql_get_define("PQL_ATTR_PPOLICY_NAME"))) { ?>
        <td class="title">
<?php	  } else { ?>
        <td class="<?php echo $class?>">
<?php	  }

		  if($ppolicies[$i][$attrib]) {
			if(($attrib == sprintf("%s", pql_get_define("PQL_ATTR_PPOLICY_MINAGE"))) or
			   ($attrib == sprintf("%s", pql_get_define("PQL_ATTR_PPOLICY_MAXAGE"))) or
			   ($attrib == sprintf("%s", pql_get_define("PQL_ATTR_PPOLICY_EXPIRE_WARNING"))) or
			   ($attrib == sprintf("%s", pql_get_define("PQL_ATTR_PPOLICY_LOCKOUT_DURATION"))) or
			   ($attrib == sprintf("%s", pql_get_define("PQL_ATTR_PPOLICY_FAILURE_COUNT_INTERVAL"))))
			{
			  $time = pql_format_timestamp_human($ppolicies[$i][$attrib]);
			  echo $time;
			} else {
			  echo $ppolicies[$i][$attrib];
			}
		  } else {
			echo "<i>".$LANG->_('Not set')."</i>";
		  }
?>
        </td>

        <td class="<?php echo $class?>" align="right">
          <a href="<?php echo $link?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo $LANG->_('Change policy value')?>"></a>
        </td>
      </tr>
<?php	} ?>

      <tr><td colspan="3"></td></tr>

<?php } ?>
      <tr>
<?php if(!$ppolicies) { ?>
        <td class="title"><?php echo $LANG->_('No policies defined')?></td>
<?php } else { ?>
        <td class="title"></td>
<?php }

      $class=pql_format_table(0);
?>
        <td class="<?php echo $class?>" colspan="2">
          <a href="config_ppolicy_add.php?rootdn=<?php echo urlencode($_SESSION["BASE_DN"][0])?>"><?php echo $LANG->_('Add policy')?></a>
        </td>
      </tr>
    </th>
  </table>
<?php

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
