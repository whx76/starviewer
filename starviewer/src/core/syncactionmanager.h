#ifndef UDGSYNCACTIONMANAGER_H
#define UDGSYNCACTIONMANAGER_H

#include <QObject>
#include <QSet>

namespace udg {

class QViewer;
class SignalToSyncActionMapper;
class SyncAction;
class SyncCriterion;
class SyncActionsConfiguration;

/**
    Class to manage SyncActions over a set of viewers.
    There will be a viewer that will act as a master viewer, set through setMasterViewer(),
    whose actions will be propagated to the rest of the registered viewers
    We can also configure which SyncActions will be propagated via setSyncActionsConfiguration()
    By default, if no configiration is provided, all registered SyncActions will be enabled
 */
class SyncActionManager : public QObject {
Q_OBJECT
public:
    SyncActionManager(QObject *parent = 0);
    ~SyncActionManager();

    /// Adds the given viewer to the set of synced viewers
    void addSyncedViewer(QViewer *viewer);

    /// Removes the given viewer from the set of synced viewers
    void removeSyncedViewer(QViewer *viewer);

    /// Sets the given viewer as the master viewer. The viewer must be already added in the set.
    void setMasterViewer(QViewer *viewer);
    
    /// Clears the synced viewers set
    void clearSyncedViewersSet();

    /// Sets the configuration for the registered sync actions
    void setSyncActionsConfiguration(SyncActionsConfiguration *configuration);

public slots:
    /// Enables or disables the manager. When disabled, no actions will be applied on the viewers set.
    /// The viewers set will always remain unaltered, wheter the manager is disabled or enabled.
    /// By default the manager will be disabled
    void enable(bool enable);

private:
    /// Sets up the signal mappers that will generate the SyncActions
    void setupSignalMappers();
    
    /// Sets up the default configuration for sync actions. It enables all the registered sync actions
    void setupDefaultSyncActionsConfiguration();
    
    /// Sets the current master viewer on the registered signal mappers
    void updateMasterViewerMappers();

    /// Checks if the given SyncAction is applicable on the given QViewer
    bool isSyncActionApplicable(SyncAction *syncAction, QViewer *viewer);

    /// Returns true if all the criteria from the list are met between the given viewer and the master one
    bool areAllCriteriaSatisfied(QList<SyncCriterion*> criteria, QViewer *viewer);

private slots:
    /// Applies the given SyncAction on the registered viewers, but the master viewer
    void applySyncAction(SyncAction *syncAction);

private:
    /// The set of viewers to be synced
    QSet<QViewer*> m_syncedViewersSet;
    
    /// The list of registered signal mappers that could be used to propagate the SyncActions
    QList<SignalToSyncActionMapper*> m_registeredSignalMappers;
    
    /// The master viewer that will set which actions should be propagated among the rest
    QViewer *m_masterViewer;

    /// Attribute that holds which sync actions are enabled or not.
    /// By default, if no SyncActionsConfiguration is provided, all registered SyncActions will be enabled
    SyncActionsConfiguration *m_syncActionsConfiguration;

    /// Attribute to hold if appliance of sync actions is enabled or not
    bool m_enabled;
};

} // End namespace udg

#endif
