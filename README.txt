Features Override 2 (Drupal 7)
------------------------------

This module add a new Features exportable called "Features Override" that is used
to override existing Features.  It requires a patch to the Features module 
(http://drupal.org/node/1317054).

To use, install the patch to Features and then install this module and enable
it.  When you create a new feature from the Structure/Features page, a new
exportable called "Features Override" will be displayed in the drop-down list.
Select this exportable and then select which overrides you wish to export to the
new feature.

This module also adds a new "Overrides" tab to the Features UI that displays the
line-by-line overrides in a structured way that makes it easier to determine where
changes have been made compared to the existing "Review Overrides" tab that requires
the Diff module.

Maintainers
-----------
- mpotter (Mike Potter)
