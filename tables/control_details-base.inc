  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">Base values</th>
<?php if($_REQUEST["mxhost"] == "Global") { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title" valign="center" align="right"><img src="images/info.png" width="16" height="16" alt="" border="0"></td>
        <td colspan="3">
          <?php echo $LANG->_('\b\uNote\B\U that any changes here, will affect \ball\B\nQmailLDAP/Control hosts in the database!')?>
        </td>
      </tr>

<?php } else { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title">RDN</td>
        <td><?php echo $_REQUEST["mxhost"]?></td>
        <td></td>
        <td></td>
      </tr>
  
      <tr class="<?php pql_format_table(); ?>">
        <td class="title">Fully qualified domain name</td>
        <td><?php echo pql_maybe_idna_decode($cn)?></td>
        <td></td>
        <td></td>
      </tr>
  
<?php } ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title">Default domainname</td>
        <td><?php echo pql_maybe_idna_decode($defaultdomain)?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?php echo $_REQUEST["mxhost"]?>&attrib=defaultdomain&type=add"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
        <td><a href="control_edit_attribute.php?mxhost=<?php echo $_REQUEST["mxhost"]?>&attrib=defaultdomain&type=del"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title">Plusdomain</td>
        <td><?php echo pql_maybe_idna_decode($plusdomain)?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?php echo $_REQUEST["mxhost"]?>&attrib=plusdomain&type=add"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
        <td><a href="control_edit_attribute.php?mxhost=<?php echo $_REQUEST["mxhost"]?>&attrib=plusdomain&type=del"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title">LDAP Server</td>
        <td><?php echo pql_maybe_idna_decode($ldapserver)?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?php echo $_REQUEST["mxhost"]?>&attrib=ldapserver&type=add"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
        <td><a href="control_edit_attribute.php?mxhost=<?php echo $_REQUEST["mxhost"]?>&attrib=ldapserver&type=del"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title">LDAP Search base</td>
        <td><?php echo pql_maybe_idna_decode($ldapbasedn)?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?php echo $_REQUEST["mxhost"]?>&attrib=ldapbasedn&type=add"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
        <td><a href="control_edit_attribute.php?mxhost=<?php echo $_REQUEST["mxhost"]?>&attrib=ldapbasedn&type=del"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title">LDAP login</td>
        <td><?php echo $ldaplogin?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?php echo $_REQUEST["mxhost"]?>&attrib=ldaplogin&type=add"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
        <td><a href="control_edit_attribute.php?mxhost=<?php echo $_REQUEST["mxhost"]?>&attrib=ldaplogin&type=del"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title">LDAP password</td>
        <td><?php echo $ldappassword?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?php echo $_REQUEST["mxhost"]?>&attrib=ldappassword&type=add"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
        <td><a href="control_edit_attribute.php?mxhost=<?php echo $_REQUEST["mxhost"]?>&attrib=ldappassword&type=del"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title">LDAP Rebind</td>
        <td><?php if($ldaprebind == 1) { echo "Yes"; $set=0; } else { echo "No"; $set=1; } ?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?php echo $_REQUEST["mxhost"]?>&attrib=ldaprebind&submit=1&type=modify&set=<?php echo $set?>"><img src="images/edit.png" width="12" height="12" alt="toggle value" border="0"></a></td>
        <td></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title">LDAP Clustering</td>
        <td><?php if($ldapcluster == 1) { echo "Yes"; $set=0; } else { echo "No"; $set=1; } ?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?php echo $_REQUEST["mxhost"]?>&attrib=ldapcluster&submit=1&type=modify&set=<?php echo $set?>"><img src="images/edit.png" width="12" height="12" alt="toggle value" border="0"></a></td>
        <td></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title">Default quota</td>
        <td><?php echo $quota?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?php echo $_REQUEST["mxhost"]?>&attrib=ldapdefaultquota&type=add"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
        <td><a href="control_edit_attribute.php?mxhost=<?php echo $_REQUEST["mxhost"]?>&attrib=ldapdefaultquota&type=del"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title">Default DOT mode</td>
        <td><?php echo $ldapdefaultdotmode?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?php echo $_REQUEST["mxhost"]?>&attrib=ldapdefaultdotmode&type=add"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
        <td><a href="control_edit_attribute.php?mxhost=<?php echo $_REQUEST["mxhost"]?>&attrib=ldapdefaultdotmode&type=del"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>

<?php
	if($dirmaker) { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title">Directory maker</td>
        <td><?php echo $dirmaker?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?php echo $_REQUEST["mxhost"]?>&attrib=dirmaker&type=add"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
        <td><a href="control_edit_attribute.php?mxhost=<?php echo $_REQUEST["mxhost"]?>&attrib=dirmaker&type=del"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>

<?php
	}

	if($quotawarning) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title">Default quota warning</td>
        <td><?php echo $quotawarning?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?php echo $_REQUEST["mxhost"]?>&attrib=quotawarning&type=add"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
        <td><a href="control_edit_attribute.php?mxhost=<?php echo $_REQUEST["mxhost"]?>&attrib=quotawarning&type=del"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>

<?php
	}
?>
    </th>
  </table>

  <br><br>

<?php
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>


