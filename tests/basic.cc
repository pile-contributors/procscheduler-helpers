#include <gtest/gtest.h>
#include <procscheduler/procscheduler.h>
#include <procscheduler/procjob_interface.h>
#include <procscheduler/procinvocation_interface.h>
#include <QApplication>

static int argc = 1;
static char *argv[] = {""};

class TestUsage : public ::testing::Test {
    QApplication app_;
public:

    TestUsage () : app_ (argc, argv)
    {}

    virtual ~TestUsage () {}

    void SetUp(){

    }
    void TearDown(){

    }

};

TEST_F(TestUsage, initial_state) {

    QString s_name = QLatin1String ("job1");
    ProcScheduler * mng = new ProcScheduler ();
    IProcJob * pjob = mng->createProcJob (mng, s_name);
    EXPECT_EQ(s_name, pjob->label ());

    EXPECT_GT(mng->concurentCount (), 0);
    mng->setConcurentCount (3);
    EXPECT_EQ(mng->concurentCount (), 3);

    EXPECT_GT(mng->jobCount (), 1);
    for (int i = 1; i < 50; ++i) {
        IProcJob * pjob = mng->createProcJob (
                    mng, QString::number (i));
        EXPECT_EQ(pjob, mng->job (i));
    }
    EXPECT_GT(mng->jobCount (), 51);

    for (int i = 1; i < 50; ++i) {
        EXPECT_EQ( mng->job (0)->manager(), mng);
        EXPECT_TRUE(mng->removeJob (0));
    }
    EXPECT_GT(mng->jobCount (), 1);

    IProcJob * ptkjob = mng->takeJob (0);
    EXPECT_GT(mng->jobCount (), 0);
    EXPECT_TRUE(ptkjob->manager() == NULL);
    delete ptkjob;

    delete mng;
}

TEST_F(TestUsage, run_one_job_one_invok) {
    QString s_name = QLatin1String ("job1");
    ProcScheduler * mng = new ProcScheduler ();
    IProcJob * pjob = mng->createProcJob (mng, s_name);
    IProcInvok * pinvok = mng->createProcInvok (
                pjob,
#               ifdef _WIN32
                "cmd", QStringList () << "/c" << "dir", QStringList ());
#               else
                "ls", QStringList () << "-la", QStringList ());
#               endif
    pjob->execute ();
    mng->waitForAll ();

    delete mng;
}

TEST_F(TestUsage, run_one_job_multi_invok) {
    QString s_name = QLatin1String ("job1");
    ProcScheduler * mng = new ProcScheduler ();
    IProcJob * pjob = mng->createProcJob (mng, s_name);
    for (int i = 0; i < 14; ++i) {
        IProcInvok * pinvok = mng->createProcInvok (
                    pjob,
#               ifdef _WIN32
                    "cmd", QStringList () << "/c" << "dir", QStringList ());
#               else
                    "ls", QStringList () << "-la", QStringList ());
#               endif
    }

    pjob->execute ();
    mng->waitForAll ();

    delete mng;
}

TEST_F(TestUsage, run_multi_job_one_invok) {
    QString s_name = QLatin1String ("job1");
    ProcScheduler * mng = new ProcScheduler ();
    for (int i = 0; i < 14; ++i) {
        IProcJob * pjob = mng->createProcJob (mng, s_name);
        IProcInvok * pinvok = mng->createProcInvok (
                    pjob,
#               ifdef _WIN32
                    "cmd", QStringList () << "/c" << "dir", QStringList ());
#               else
                    "ls", QStringList () << "-la", QStringList ());
#               endif
        pjob->execute ();
    }
    mng->waitForAll ();

    delete mng;
}

TEST_F(TestUsage, run_multi_job_multi_invok) {
    QString s_name = QLatin1String ("job1");
    ProcScheduler * mng = new ProcScheduler ();
    for (int i = 0; i < 14; ++i) {
        IProcJob * pjob = mng->createProcJob (mng, s_name);
        for (int j = 0; j < 14; ++j) {
            IProcInvok * pinvok = mng->createProcInvok (
                        pjob,
#                   ifdef _WIN32
                        "cmd", QStringList () << "/c" << "dir", QStringList ());
#                   else
                        "ls", QStringList () << "-la", QStringList ());
#                   endif
        }
        pjob->execute ();
    }
    mng->waitForAll ();

    delete mng;
}

