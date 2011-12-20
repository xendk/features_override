Features Field Override (Drupal 7)
-----------------------

This module add a new Features exportable called "Field Settings" that is used
to export the Display and Widget settings of fields.  It requires a patch to 
the Features module (http://drupal.org/node/1277854).

To use, install the patch to Features and then install this module and enable
it.  When you create a new feature from the Structure/Features page, a new
exportable called "Field Settings" will be displayed in the drop-down list.
Select this exportable and then select which fields you wish to export the
display settings for.

Any display settings exported by this module will override any display settings
exported by the normal Field exportable.  This allows for easier separation 
between base/distribution features and site-specific display settings.

Tutorial Blog article: http://www.agileapproach.com/blog-entry/new-paradigm-overriding-drupal-features

Maintainers
-----------
- mpotter (Mike Potter)
