SPECS += foc_arm zybo
include $(call select_from_repositories,mk/spec/zybo.mk)
include $(call select_from_repositories,mk/spec/foc_arm.mk)
