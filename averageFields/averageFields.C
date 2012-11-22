/**
 * \file
 * \author Dmitri Naumov (dmitri.naumov@ufz.de)
 *
 * \copyright 2012 -- Helmholtz Centre for Environmental Research - UFZ
 * distributed under the GNU GPL version 3. See the LICENSE file for details.
 *
 * \brief Time averages of fields.
 *
 * Calculates time averages of U and p over a time span writing result to time
 * zero.
 */

#include "fvCFD.H"

int
main(int argc, char *argv[])
{
    timeSelector::addOptions();

    #include "setRootCase.H"
    #include "createTime.H"

    instantList timeDirs = timeSelector::select0(runTime, args);

    #include "createMesh.H"

    runTime.setTime(timeDirs.last(), timeDirs.size()-1);

    #include "createFields.H"

    int n = 0;

    forAll(timeDirs, timeI)
    {
        runTime.setTime(timeDirs[timeI], timeI);

        Info<< "Time = " << runTime.timeName() << endl;

        IOobject pheader
        (
            "p",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ
        );

        if (!pheader.headerOk())
        {
            Info << "Unable to read p" << endl;
            return EXIT_FAILURE;
        }

        Info << "    Reading p" << endl;
        volScalarField p(pheader, mesh);

        IOobject Uheader
        (
            "U",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ
        );

        if (!Uheader.headerOk())
        {
            Info << "Unable to read U" << endl;
            return EXIT_FAILURE;
        }

        Info << "    Reading U" << endl;
        volVectorField U(Uheader, mesh);


        n++;
        pMean += (p - pMean)/n;
        UMean += (U - UMean)/n;

        Info<< endl;
    }

    // Write result to time 0
    runTime.setTime(0, 0);
    pMean.write();
    UMean.write();

    return 0;
}
